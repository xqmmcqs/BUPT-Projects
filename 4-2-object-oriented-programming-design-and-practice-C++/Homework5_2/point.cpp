/**
 * @file      point.cpp
 * @brief     点
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是点重载前置和后置自增、自减运算符以及友元输出函数的实现。
*/

#include "point.h"

Point &Point::operator++()
{
    ++x;
    ++y;
    return *this;
}

Point Point::operator++(int)
{
    Point ret = *this;
    ++x;
    ++y;
    return ret;
}

Point &Point::operator--()
{
    --x;
    --y;
    return *this;
}

Point Point::operator--(int)
{
    Point ret = *this;
    --x;
    --y;
    return ret;
}

std::ostream &operator<<(std::ostream &os, Point p)
{
    os << p.x << ' ' << p.y;
    return os;
}
