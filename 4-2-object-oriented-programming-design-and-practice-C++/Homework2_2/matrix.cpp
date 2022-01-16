/**
 * @file      matrix.cpp
 * @brief     矩阵
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是矩阵的实现。
 *
 * 本文件中实现了矩阵的构造、拷贝构造、移动构造、输入、输出、析构函数，并且重载了移动赋值、拷贝赋值、加、减和下标运算符。
*/

#include "matrix.h"

using namespace std;

Matrix::Matrix(int l, int r) : lines(l), rows(r)
{
    try
    {
        value = unique_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[l]);
        for (int i = 0; i < l; ++i)
            value[i] = unique_ptr<int[]>(new int[r]);
    }
    catch (const bad_alloc &e)
    {
        value = nullptr;
        exit(1);
    }
}

Matrix::Matrix(const Matrix &matrix) : rows(matrix.rows), lines(matrix.lines)
{
    try
    {
        value = unique_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[lines]);
        for (int i = 0; i < lines; ++i)
            value[i] = unique_ptr<int[]>(new int[rows]);
    }
    catch (const bad_alloc &e)
    {
        value = nullptr;
        exit(1);
    }
}

Matrix::Matrix(Matrix &&matrix) noexcept: rows(matrix.rows), lines(matrix.lines), value(matrix.value.release())
{
    matrix.lines = 0;
    matrix.rows = 0;
}

Matrix &Matrix::operator=(const Matrix &matrix)
{
    if (this != &matrix)
    {
        this->~Matrix();
        rows = matrix.rows;
        lines = matrix.lines;
        try
        {
            value = unique_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[lines]);
            for (int i = 0; i < lines; ++i)
            {
                value[i] = unique_ptr<int[]>(new int[rows]);
                for (int j = 0; j < lines; ++j)
                    value[i][j] = matrix[i][j];
            }
        }
        catch (const bad_alloc &e)
        {
            value = nullptr;
            exit(1);
        }
    }
    return *this;
}

Matrix &Matrix::operator=(Matrix &&matrix) noexcept
{
    if (this != &matrix)
    {
        this->~Matrix();
        rows = matrix.rows;
        lines = matrix.lines;
        value.reset(matrix.value.release());
        matrix.lines = matrix.rows = 0;
    }
    return *this;
}


void Matrix::input(istream &is)
{
    cout << "Please input a matrix, size " << rows << " * " << lines << ":" << endl;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < lines; ++j)
            is >> value[i][j];
}

void Matrix::output(ostream &os) const
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < lines; ++j)
            os << value[i][j] << ' ';
        os << endl;
    }
}

Matrix Matrix::operator+(const Matrix &addend2) const
{
    if (rows != addend2.rows || lines != addend2.lines)return Matrix(0, 0);
    Matrix result(rows, lines);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < lines; ++j)
            result.value[i][j] = value[i][j] + addend2[i][j];
    return result;
}

Matrix Matrix::operator-(const Matrix &subtractor) const
{
    if (rows != subtractor.rows || lines != subtractor.lines)return Matrix(0, 0);
    Matrix result(rows, lines);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < lines; ++j)
            result.value[i][j] = value[i][j] - subtractor[i][j];
    return result;
}

unique_ptr<int[]> &Matrix::operator[](const int x)
{
    return value[x];
}

const unique_ptr<int[]> &Matrix::operator[](const int x) const
{
    return value[x];
}

Matrix::~Matrix()
{
    value.reset();
}
