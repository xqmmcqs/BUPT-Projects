/**
 * @file      circle.h
 * @brief     圆
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是点和圆的数据结构和接口。
 *
 * Point类表示点，其中声明求两点距离的方法。
 *
 * Circle类表示圆，其中声明判断两圆是否重叠的方法。
*/

#ifndef HOMEWORK_2_1_CIRCLE_H
#define HOMEWORK_2_1_CIRCLE_H

#include <cmath>
#include <iostream>

class Point
{
public:
    double x, y;
    
    /**
     * @brief 点的构造函数，默认点在(0,0)
     */
    Point(double _x = 0, double _y = 0) : x(_x), y(_y)
    {
#ifdef DEBUG
        std::cout << "Constructing a point at (" << x << "," << y << ")." << std::endl;
#endif
    }
    
    ~Point()
    {
#ifdef DEBUG
        std::cout << "Destroying a point at (" << x << "," << y << ")." << std::endl;
#endif
    }
    
    /**
     * @brief 求两点的距离
     * @param point 另一个点
     */
    double get_dis(const Point &point) const;
};

class Circle
{
public:
    Point center;
    double radius;
    
    Circle(double _x, double _y, double _r) : radius(_r), center(_x, _y)
    {
#ifdef DEBUG
        std::cout << "Constructing a circle at (" << center.x << "," << center.y << ") and radius = " << radius << "." << std::endl;
#endif
    }
    
    ~Circle()
    {
#ifdef DEBUG
        std::cout << "Destroying a circle at (" << center.x << "," << center.y << ") and radius = " << radius << "." << std::endl;
#endif
    }
    
    /**
     * @brief 判断两圆是否重叠
     * @param circle 另一个圆
     * @return 如果两圆重叠，返回true，否则返回false
     * @note 如果两圆相切，认为它们重叠
     */
    bool is_overlap(const Circle &circle) const;
};

#endif //HOMEWORK_2_1_CIRCLE_H
