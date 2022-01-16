/**
 * @file      matrix.cpp
 * @brief     矩阵
 * @author    Ziheng Mao
 * @date      2021/3/17
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是矩阵初始化（分配内存）、输入矩阵、输出矩阵、矩阵的加法、矩阵的减法、矩阵空间释放函数的实现。
*/

#include "matrix.h"

using namespace std;

void init_Matrix(Matrix &matrix)
{
    try
    {
        matrix.value = unique_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[4]);
        for (int i = 0; i < 4; ++i)
            matrix.value[i] = unique_ptr<int[]>(new int[5]);
    }
    catch (const bad_alloc &e)
    {
        matrix.value = nullptr;
        exit(1);
    }
}

void input_Matrix(Matrix &matrix)
{
    cout << "Please input a matrix, size 4 * 5:" << endl;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
            cin >> matrix.value[i][j];
}

void output_Matrix(const Matrix &matrix)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
            cout << matrix.value[i][j] << ' ';
        cout << endl;
    }
}

void add_Matrix(const Matrix &addend1, const Matrix &addend2, Matrix &result)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
            result.value[i][j] = addend1.value[i][j] + addend2.value[i][j];
}

void sub_Matrix(const Matrix &subtrahend, const Matrix &subtractor, Matrix &result)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
            result.value[i][j] = subtrahend.value[i][j] - subtractor.value[i][j];
}

void release_Matrix(Matrix &matrix)
{
    matrix.value.reset();
}




