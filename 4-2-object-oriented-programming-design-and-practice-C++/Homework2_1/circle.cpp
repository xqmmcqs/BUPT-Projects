/**
 * @file      circle.cpp
 * @brief     圆
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是求两点距离和判断两圆是否重叠函数的实现。
*/

#include "circle.h"

double Point::get_dis(const Point &point) const
{
    return sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y));
}

bool Circle::is_overlap(const Circle &circle) const
{
    return center.get_dis(circle.center) <= radius + circle.radius;
}
