/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中实现了一个猜数游戏。
*/

#include <iostream>
#include <ctime>
#include <random>

using namespace std;

int main()
{
    uniform_int_distribution<int> u(1, 1000);
    default_random_engine e(time(nullptr));
    int ans = u(e);
    while (true)
    {
        double guess;
        cout << "Please input a real number (range 1 to 1000)" << endl;
        while (true)
        {
            cin >> guess;
            if (cin.fail())
            {
                cin.clear();
                cin.get();
                cout << "Please input a real number (range 1 to 1000)" << endl;
                continue;
            }
            else break;
        }
        if (guess < 1 || guess > 1000)
        {
            cout << "The entered value is out of range." << endl;
            continue;
        }
        if (guess < ans)
            cout << "Too small." << endl;
        else if (guess > ans)
            cout << "Too large." << endl;
        else
        {
            cout << "Correct." << endl;
            break;
        }
    }
    return 0;
}