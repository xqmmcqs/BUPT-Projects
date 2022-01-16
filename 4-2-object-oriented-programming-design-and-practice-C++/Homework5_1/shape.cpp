/**
 * @file      shape.cpp
 * @brief     形状
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是矩形和圆求面积函数的实现。
*/

#include "shape.h"

double Rectangle::area() const
{
    return r * h;
}

double Circle::area() const
{
    return PI * r * r;
}
