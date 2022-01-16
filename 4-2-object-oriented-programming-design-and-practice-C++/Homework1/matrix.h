/**
 * @file      matrix.h
 * @brief     矩阵
 * @author    Ziheng Mao
 * @date      2021/3/17
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是矩阵的数据结构和接口。
 *
 * 文件中声明了矩阵初始化（分配内存）、输入矩阵、输出矩阵、矩阵的加法、矩阵的减法、矩阵空间释放函数的接口。
*/

#ifndef HOMEWORK_1_MATRIX_H
#define HOMEWORK_1_MATRIX_H

#include <memory>
#include <iostream>

/**
 * @brief 矩阵结构体
 */
struct Matrix
{
    std::unique_ptr<std::unique_ptr<int[]>[]> value;
};

/**
 * @brief 初始化矩阵，为value成员分配4*5的空间
 * @note 一个Matrix对象应当在创建之后调用本函数，并且在释放之前只调用一次本函数
 */
void init_Matrix(Matrix &matrix);

/**
 * @brief 从标准输入中读入矩阵
 */
void input_Matrix(Matrix &matrix);

/**
 * @brief 向标准输出中输出矩阵
 */
void output_Matrix(const Matrix &matrix);

/**
 * @brief 将两个矩阵相加
 * @param addend1 加数
 * @param addend2 加数
 * @param result 矩阵相加的和
 */
void add_Matrix(const Matrix &addend1, const Matrix &addend2, Matrix &result);

/**
 * @brief 将两个矩阵相减
 * @param subtrahend 被减数
 * @param subtractor 减数
 * @param result 矩阵相减的差
 */
void sub_Matrix(const Matrix &subtrahend, const Matrix &subtractor, Matrix &result);

/**
 * @brief 释放矩阵，将matrix置为空指针
 * @note 矩阵在释放之前应当调用过初始化函数
 */
void release_Matrix(Matrix &matrix);

#endif //HOMEWORK_1_MATRIX_H
