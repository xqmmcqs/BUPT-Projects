import asyncio
import math
import os
import numpy as np
from sanic import Sanic
from sanic.response import json
from sanic.log import logger
from shapely.geometry import Polygon
from shapely.errors import TopologicalError

app = Sanic("app")


@app.listener("before_server_start")
async def setup(app, loop):
    for i in range(1, 9):
        step = 10  # 处理成10度*10度的block
        with open(f"./gpw-v4-population-count-rev11_2020_30_sec_asc/gpw_v4_population_count_rev11_2020_30_sec_{i}.asc",
                  "r") as f:
            f.readline()
            f.readline()
            stx = int(float(f.readline().split()[1]))  # 左上角经度
            sty = int(float(f.readline().split()[1])) + 90  # 左上角纬度
            cooked = True  # 该部分中的所有block是否已经被处理
            for x_offset in range(0, 90, step):
                for y_offset in range(0, 90, step):
                    if not os.path.exists(f"./data/data_{stx + x_offset}_{sty - y_offset}.npy"):
                        cooked = False
        logger.info(f"Processing data on x: {stx} y: {sty}")
        if cooked:
            continue
        data = np.genfromtxt(
            f"./gpw-v4-population-count-rev11_2020_30_sec_asc/gpw_v4_population_count_rev11_2020_30_sec_{i}.asc",
            skip_header=6)
        data[data == -9999] = np.nan  # 空数据
        for x_offset in range(0, 90, step):
            for y_offset in range(0, 90, step):
                if not os.path.exists(f"./data/data_{stx + x_offset}_{sty - y_offset}.npy"):
                    logger.info(f"Creating data_{stx + x_offset}_{sty - y_offset}.npy")
                    np.save(f"./data/data_{stx + x_offset}_{sty - y_offset}.npy",
                            data[y_offset * 120:(y_offset + step) * 120, x_offset * 120:(x_offset + step) * 120])


async def get_block_data(block_x, block_y, polygon, step):
    logger.info(f"Query on block x: {block_x} y: {block_y}")
    res = []
    total = 0
    minx, miny, maxx, maxy = polygon.bounds
    block_data = np.load(f"./data/data_{block_x}_{block_y}.npy")
    for second_x in range(max(block_x * 3600, math.floor(minx / 30) * 30),
                          min((block_x + step) * 3600, math.ceil(maxx / 30) * 30), 30):  # 按每30秒枚举经度
        for second_y in range(min(block_y * 3600, math.ceil(maxy / 30) * 30),
                              max((block_y - step) * 3600, math.floor(miny / 30) * 30), -30):  # 按每30秒枚举纬度
            x_offset = int((block_y * 3600 - second_y) / 30)  # 在block中的行偏移量
            y_offset = int((second_x - block_x * 3600) / 30)  # 在block中的列偏移量
            cell_polygon = Polygon(((second_x, second_y), (second_x + 30, second_y), (second_x + 30, second_y - 30),
                                    (second_x, second_y - 30))).intersection(polygon)  # 该cell和多边形的重合部分
            if cell_polygon.area > 0:  # 如果有重合
                res.append((second_x, second_y, cell_polygon.area / 900 * block_data[x_offset, y_offset]))
                if not np.isnan(res[-1][2]):
                    total += res[-1][2]
    return res, total


@app.post("/data")
async def get_data(request):
    try:
        point_list = request.json.get("coordinates")
        polygon = Polygon(point_list)
        logger.info(f"Query prarms: {point_list}")
        minx, miny, maxx, maxy = polygon.bounds
        step = 10  # 一个block跨10度
        res = []
        total = 0
        task_list = []
        for block_x in range(math.floor(minx / 3600 / step) * step, math.ceil(maxx / 3600 / step) * step, step):
            for block_y in range(math.ceil(maxy / 3600 / step) * step, math.floor(miny / 3600 / step) * step, -step):
                task_list.append(
                    asyncio.create_task(get_block_data(block_x, block_y, polygon, step)))  # 每个block中的查询并行处理
        for task in task_list:
            res_, total_ = await task
            res += res_
            total += total_
        return json({"total": total, "res": res})
    except KeyError:  # 参数错误
        return json([], status=400)
    except (ValueError, TopologicalError):  # 非多边形
        return json([], status=406)


if __name__ == '__main__':
    app.run()
