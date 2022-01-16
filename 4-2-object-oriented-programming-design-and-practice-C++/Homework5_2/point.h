/**
 * @file      point.h
 * @brief     点
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是点的数据结构和接口。
 *
 * 本文件中重载了点的前置和后置自增、自减运算符，并且声明了一个友元输出函数。
*/

#ifndef HOMEWORK_5_2_POINT_H
#define HOMEWORK_5_2_POINT_H

#include <iostream>

class Point
{
private:
    int x, y;

public:
    Point(int _x, int _y) : x(_x), y(_y)
    {}
    
    ~Point() = default;
    
    Point &operator++();
    
    Point operator++(int);
    
    Point &operator--();
    
    Point operator--(int);
    
    friend std::ostream &operator<<(std::ostream &os, Point p);
};

std::ostream &operator<<(std::ostream &os, Point p);

#endif //HOMEWORK_5_2_POINT_H
