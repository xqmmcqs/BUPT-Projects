/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中的定义了两个圆对象，并且判断它们是否重叠。
*/

#include <iostream>
#include "circle.h"

using namespace std;

int main()
{
    int x, y, r;
    cout << "Pleast input the center of the first circle." << endl;
    if (cin.peek() == '\n')
        x = y = 0;
    else cin >> x >> y;
    cout << "Pleast input the radius of the first circle." << endl;
    cin >> r;
    if (r <= 0)
    {
        cout << "The radius should be a positive integer." << endl;
        return 1;
    }
    cin.get();
    Circle c1(x, y, r);
    cout << "Pleast input the center of the second circle." << endl;
    if (cin.peek() == '\n')
        x = y = 0;
    else cin >> x >> y;
    cout << "Pleast input the radius of the second circle." << endl;
    cin >> r;
    if (r <= 0)
    {
        cout << "The radius should be a positive integer." << endl;
        return 1;
    }
    Circle c2(x, y, r);
    if (c1.is_overlap(c2))cout << "overlap" << endl;
    else cout << "not overlap" << endl;
    return 0;
}