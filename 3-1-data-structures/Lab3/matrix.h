//
// Created by xqmmcqs on 2020/10/24.
//

#ifndef LAB3_MATRIX_H
#define LAB3_MATRIX_H

#define MATRIXINCREASESIZE 100000

typedef struct
{
    int i, j;
    long long val;
} Tuple;

typedef struct
{
    Tuple * data;
    int * pos; // 每一行中首个非零元素的位置
    int n, m, tot; // tot为非空元素总数
    int sizeOfMatrix; // 三元组存储单位的个数
} Matrix;

/*
 * 操作：初始化矩阵
 * 前件：a指向一个空矩阵，n>0，m>0
 * 后件：a指向一个n*m的零矩阵
 */
void initMatrix(Matrix * a, int n, int m);

/*
 * 操作：扩展矩阵的存储空间
 * 前件：c指向一个矩阵
 * 后件：该矩阵扩展MATRIXINCREASESIZE个三元组存储单位
 */
void expandMatrix(Matrix * c);

/*
 * 操作：把由一维数组存储的矩阵转化为由三元组表存储的矩阵
 * 前件：n>0，m>0，val中存储一个矩阵，矩阵元素a[i][j]存储在val[(i-1)*m+j]中
 * 后件：函数返回由三元组表作为存储形式的矩阵
 */
Matrix array2Matrix(int n, int m, long long val[]);

/*
 * 操作：把两个矩阵相加
 * 前件：a,b为两个矩阵，并且行数和列数都相等
 * 后件：函数返回两个矩阵相加的结果
 */
Matrix addMatrix(Matrix a, Matrix b);

/*
 * 操作：把两个矩阵相乘
 * 前件：a,b为两个矩阵，并且a的列数和b的行数相等
 * 后件：函数返回两个矩阵相乘的结果
 */
Matrix mulMatrix(Matrix a, Matrix b);

/*
 * 操作：把由三元组表存储的矩阵转化为由一维数组存储的矩阵
 * 前件：a为一个矩阵，val指向一个至少有n*m+1个存储单位的long long类型的数组
 * 后件：val指向由一维数组存储的矩阵，矩阵元素a[i][j]存储在val[(i-1)*m+j]中
 */
void matrix2Array(Matrix a, long long val[]);

/*
 * 操作：释放矩阵空间
 * 前件：a指向一个矩阵
 * 后件：a指向一个空矩阵
 */
void destroyMatrix(Matrix * a);

#endif //LAB3_MATRIX_H