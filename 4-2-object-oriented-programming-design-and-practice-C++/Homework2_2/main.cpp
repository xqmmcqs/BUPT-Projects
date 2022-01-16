/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中的定义了两个矩阵对象，输入，分别求它们的和、差并且输出，之后定义两个指向矩阵对象的指针，输入，分别求它们的和、差并且输出。
*/

#include <iostream>

#include "matrix.h"

using namespace std;

int main()
{
    int n, m;
    cout << "Please input the number of rows and columns of the first matrix:" << endl;
    cin >> n >> m;
    Matrix A1(n, m);
    A1.input(cin);
    cout << "Please input the number of rows and columns of the second matrix:" << endl;
    cin >> n >> m;
    Matrix A2(n, m);
    A2.input(cin);
    
    Matrix A3;
    A3 = A1 + A2;
    cout << "The sum of the matrices is:" << endl;
    A3.output(cout);
    A3 = A1 - A2;
    cout << "The difference of the matrices is:" << endl;
    A3.output(cout);
    
    cout << "Please input the number of rows and columns of the third matrix:" << endl;
    cin >> n >> m;
    auto * pA1 = new Matrix(n, m);
    pA1->input(cin);
    cout << "Please input the number of rows and columns of the forth matrix:" << endl;
    cin >> n >> m;
    auto * pA2 = new Matrix(n, m);
    pA2->input(cin);
    
    auto * pA3 = new Matrix;
    *pA3 = *pA1 + *pA2;
    cout << "The sum of the matrices is:" << endl;
    pA3->output(cout);
    *pA3 = *pA1 - *pA2;
    cout << "The difference of the matrices is:" << endl;
    pA3->output(cout);
    
    delete pA1;
    delete pA2;
    delete pA3;
    return 0;
}
