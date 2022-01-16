/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/3/17
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了三个矩阵对象，输入两个矩阵，分别求其和、差并输出，之后释放其空间。
*/

#include "matrix.h"

int main()
{
    Matrix A1{}, A2{}, A3{};
    init_Matrix(A1);
    init_Matrix(A2);
    init_Matrix(A3);
    
    input_Matrix(A1);
    input_Matrix(A2);
    
    add_Matrix(A1, A2, A3);
    cout << "The sum of the matrices is:" << endl;
    output_Matrix(A3);
    
    sub_Matrix(A1, A2, A3);
    cout << "The difference of the matrices is:" << endl;
    output_Matrix(A3);
    
    release_Matrix(A1);
    release_Matrix(A2);
    release_Matrix(A3);
    return 0;
}
