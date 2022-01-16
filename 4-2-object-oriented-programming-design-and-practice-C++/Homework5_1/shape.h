/**
 * @file      shape.h
 * @brief     形状
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是形状的数据结构和接口。
 *
 * 本文件中声明了形状类、矩形类、圆形类、正方形类，并且声明了矩形、圆形的求面积方法。
*/

#ifndef HOMEWORK_5_1_SHAPE_H
#define HOMEWORK_5_1_SHAPE_H

#include <iostream>
#include <cmath>

const double PI = acos(-1);

class Shape
{
protected:
    virtual double area() const = 0;

public:
    Shape()
    {
#ifdef DEBUG
        std::cout << "Constructing a shape." << std::endl;
#endif
    }
    
    virtual ~Shape()
    {
#ifdef DEBUG
        std::cout << "Destroying a shape." << std::endl;
#endif
    }
};

class Rectangle : public Shape
{
protected:
    double r, h;

public:
    Rectangle(double _r, double _h) : r(_r), h(_h)
    {
#ifdef DEBUG
        std::cout << "Constructing a rectangle with r = " << r << " and h = " << h << std::endl;
#endif
    }
    
    ~Rectangle()
    {
#ifdef DEBUG
        std::cout << "Destroying a rectangle with r = " << r << " and h = " << h << std::endl;
#endif
    }
    
    double area() const final;
};

class Circle : public Shape
{
private:
    double r;

public:
    Circle(double _r) : r(_r)
    {
#ifdef DEBUG
        std::cout << "Constructing a circle with r = " << r << std::endl;
#endif
    }
    
    ~Circle()
    {
#ifdef DEBUG
        std::cout << "Destroying a circle with r = " << r << std::endl;
#endif
    }
    
    double area() const final;
};

class Square : public Rectangle
{
public:
    Square(double _a) : Rectangle(_a, _a)
    {
#ifdef DEBUG
        std::cout << "Constructing a square with a = " << r << std::endl;
#endif
    }
    
    ~Square()
    {
#ifdef DEBUG
        std::cout << "Destroying a square with a = " << r << std::endl;
#endif
    }
};

#endif //HOMEWORK_5_1_SHAPE_H
