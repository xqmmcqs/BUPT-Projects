/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中的定义了一个点对象，观察前置和后置自增自减运算符的区别。
*/

#include <iostream>
#include "point.h"

using namespace std;

int main()
{
    int x, y;
    cout << "Please input the position of the point:" << endl;
    cin >> x >> y;
    Point p(x, y);
    cout << ++p << ' ' << p << endl;
    cout << p++ << ' ' << p << endl;
    cout << --p << ' ' << p << endl;
    cout << p-- << ' ' << p << endl;
    return 0;
}