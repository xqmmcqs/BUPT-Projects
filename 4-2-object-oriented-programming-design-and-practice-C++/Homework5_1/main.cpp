/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中的定义了一个正方形对象和圆对象，输入，分别求它们的面积并输出。
*/

#include "shape.h"

using namespace std;

int main()
{
    double r;
    while (true)
    {
        cout << "Please input the side length of a square." << endl;
        cin >> r;
        if (r > 0)break;
    }
    Square squ(r);
    while (true)
    {
        cout << "Please input the radius of a circle." << endl;
        cin >> r;
        if (r > 0)break;
    }
    Circle cir(r);
    cout << "The area of the square is " << squ.area() << endl;
    cout << "The area of the circle is " << cir.area() << endl;
    return 0;
}