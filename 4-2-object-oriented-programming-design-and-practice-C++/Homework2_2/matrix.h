/**
 * @file      matrix.h
 * @brief     矩阵
 * @author    Ziheng Mao
 * @date      2021/3/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是矩阵的数据结构和接口。
 *
 * 本文件中声明了矩阵的构造、输入、输出、析构函数，并且重载了赋值和加、减运算符。
*/

#ifndef HOMEWORK_2_2_MATRIX_H
#define HOMEWORK_2_2_MATRIX_H

#include <iostream>
#include <memory>

class Matrix
{
private:
    int lines, rows;
    std::unique_ptr<std::unique_ptr<int[]>[]> value;
    
    std::unique_ptr<int[]> &operator[](int x);
    
    const std::unique_ptr<int[]> &operator[](int x) const;

public:
    Matrix() : lines(0), rows(0), value(nullptr)
    {}
    
    Matrix(int l, int r);
    
    Matrix(const Matrix &matrix);
    
    Matrix(Matrix &&matrix) noexcept;
    
    Matrix &operator=(const Matrix &matrix);
    
    Matrix &operator=(Matrix &&matrix) noexcept;
    
    /**
     * @brief 从指定输入流中输入矩阵
     */
    void input(std::istream &is);
    
    /**
     * @brief 从指定输出流中输出矩阵
     */
    void output(std::ostream &os) const;
    
    /**
     * @brief 计算两个矩阵的和
     * @note 如果两个矩阵大小不同，则返回空矩阵
     */
    Matrix operator+(const Matrix &addend2) const;
    
    /**
     * @brief 计算两个矩阵的差
     * @note 如果两个矩阵大小不同，则返回空矩阵
     */
    Matrix operator-(const Matrix &subtractor) const;
    
    ~Matrix();
};

#endif //HOMEWORK_2_2_MATRIX_H
