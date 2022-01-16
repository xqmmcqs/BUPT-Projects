import math
import sys
import logging
import requests
import numpy as np
import matplotlib.pyplot as plt
import cartopy.crs as ccrs
import cartopy.feature as cfeature
from PyQt5.QtCore import QObject, pyqtProperty, pyqtSlot, pyqtSignal
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import QQmlApplicationEngine, QQmlListProperty


class Coordinate(QObject):
    def __init__(self, x: int, y: int, parent=None) -> None:
        super().__init__(parent)
        self.x = x
        self.y = y

    def __str__(self):
        return f"[{self.x},{self.y}]"

    @pyqtProperty(int, constant=True)
    def x_deg(self) -> int:
        return self.x // 3600

    @pyqtProperty(int, constant=True)
    def x_min(self) -> int:
        return self.x % 3600 // 60

    @pyqtProperty(int, constant=True)
    def x_sec(self) -> int:
        return self.x % 60

    @pyqtProperty(int, constant=True)
    def y_deg(self) -> int:
        return self.y // 3600

    @pyqtProperty(int, constant=True)
    def y_min(self) -> int:
        return self.y % 3600 // 60

    @pyqtProperty(int, constant=True)
    def y_sec(self) -> int:
        return self.y % 60


# noinspection PyUnresolvedReferences
class ClientModel(QObject):
    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self._coordinate_list = []

    coordinate_list_changed = pyqtSignal()

    @pyqtProperty(QQmlListProperty, notify=coordinate_list_changed)
    def coordinate_list(self) -> QQmlListProperty:
        return QQmlListProperty(Coordinate, self, self._coordinate_list)

    @pyqtSlot(int, int)
    def add_point(self, x: int, y: int) -> None:
        logging.info(f"Add point x: {x}, y: {y}")
        self._coordinate_list.append(Coordinate(x, y))
        self.coordinate_list_changed.emit()

    @pyqtSlot(int)
    def remove_point(self, index: int) -> None:
        logging.info(f"Remove point index: {index}")
        self._coordinate_list.pop(index)
        self.coordinate_list_changed.emit()

    @pyqtSlot()
    def submit(self) -> None:
        if len(self._coordinate_list) == 0:  # 消息为空
            return
        logging.info(f"Submit points")
        r = requests.post("http://127.0.0.1:8000/data",
                          json={"type": "Polygon", "coordinates": [[a.x, a.y] for a in self._coordinate_list]})
        if r.status_code == 406:
            root.errorOccurred("输入有误，必须为多边形")
            return
        elif r.status_code != 200:
            root.errorOccurred("服务器错误")
            return
        self._coordinate_list.clear()
        self.coordinate_list_changed.emit()
        if len(r.json().get("res")) == 0:
            return
        data = np.array(r.json().get("res")).transpose((1, 0))
        extent = [math.floor(np.min(data[0]) / 3600), math.ceil(np.max(data[0]) / 3600),
                  math.floor(np.min(data[1]) / 3600), math.ceil(np.max(data[1]) / 3600)]  # 绘图范围
        fig = plt.figure(figsize=(8, 6))
        ax = fig.add_subplot(111, projection=ccrs.PlateCarree())
        ax.set_extent(extent, crs=ccrs.PlateCarree())  # 设置范围
        ax.add_feature(cfeature.LAND.with_scale('10m'))  # 图背景的陆地标识
        ax.add_feature(cfeature.COASTLINE.with_scale('10m'), lw=0.25)  # 图背景的海岸线标识
        ax.add_feature(cfeature.OCEAN.with_scale('10m'))  # 图背景的海洋标识
        ax.gridlines(draw_labels=True, dms=True, x_inline=False, y_inline=False)
        im = ax.scatter([i / 3600 for i in data[0]], [i / 3600 for i in data[1]], s=0.5, c=data[2], cmap='viridis')
        fig.colorbar(im, ax=ax)
        ax.title.set_text(f"Total population of the area is {r.json().get('total'):.2f}")
        plt.show()


if __name__ == '__main__':
    logging.basicConfig(format='%(asctime)s %(message)s', level=logging.INFO)
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    client_model = ClientModel()
    engine.rootContext().setContextProperty("client_model", client_model)
    engine.load('main.qml')
    root = engine.rootObjects()[0]
    app.exec()
