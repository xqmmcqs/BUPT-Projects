//
// Created by xqmmcqs on 2020/10/24.
//

#include <string.h>
#include <stdlib.h>
#include "matrix.h"

// 初始化矩阵
void initMatrix(Matrix * a, int n, int m)
{
    a->n = n;
    a->m = m;
    a->tot = 0;
    if (!(a->pos = (int *) calloc(n + 2, sizeof(int))))
        exit(1);
    a->sizeOfMatrix = MATRIXINCREASESIZE;
    if (!(a->data = (Tuple *) calloc(MATRIXINCREASESIZE, sizeof(Tuple))))
        exit(1);
}

// 扩展矩阵的存储空间
void expandMatrix(Matrix * c)
{
    // 分配更多存储空间
    if (!(c->data = (Tuple *) realloc(c->data, (c->sizeOfMatrix + MATRIXINCREASESIZE) * sizeof(Tuple))))
        exit(1);
    c->sizeOfMatrix += MATRIXINCREASESIZE;
}

// 把由一维数组存储的矩阵转化为由三元组表存储的矩阵
Matrix array2Matrix(int n, int m, long long val[])
{
    Matrix a;
    initMatrix(&a, n, m);
    for (int i = 1; i <= n; ++i)
    {
        a.pos[i] = a.tot + 1;
        for (int j = 1; j <= m; ++j)
        {
            if (val[(i - 1) * m + j])
            {
                if (a.tot + 1 >= a.sizeOfMatrix)
                    expandMatrix(&a);
                a.data[++a.tot] = (Tuple) {i, j, val[(i - 1) * m + j]};
            }
        }
    }
    a.pos[n + 1] = a.tot + 1;
    return a;
}

// 把两个矩阵相加
Matrix addMatrix(Matrix a, Matrix b)
{
    if (a.n != b.n || a.m != b.m) // 两个矩阵不可相加
        exit(1);
    Matrix c;
    initMatrix(&c, a.n, b.m);
    if (!a.tot || !b.tot) // 如果有一个矩阵为空，则返回空矩阵
        return c;
    for (int i = 1; i <= a.n; ++i)
    {
        c.pos[i] = c.tot + 1;
        int p1 = a.pos[i], p2 = b.pos[i];
        while (p1 < a.pos[i + 1] || p2 < b.pos[i + 1]) // 枚举a矩阵和b矩阵第i行的非零元素
        {
            int tempj;
            long long tempv;
            if (p2 >= b.pos[i + 1] ||
                p1 < a.pos[i + 1] && a.data[p1].j < b.data[p2].j) // 如果b矩阵本行没有元素或者a矩阵非零元素列数小于b矩阵非零元素的列数，c[i][j]=a[i][j]
            {
                tempj = a.data[p1].j;
                tempv = a.data[p1].val;
                ++p1;
            }
            else if (p1 >= a.pos[i + 1] ||
                     p2 < b.pos[i + 1] && a.data[p1].j > b.data[p2].j) // 如果a矩阵本行没有元素或者b矩阵非零元素列数小于a矩阵非零元素的列数，c[i][j]=b[i][j]
            {
                tempj = b.data[p2].j;
                tempv = b.data[p2].val;
                ++p2;
            }
            else // c[i][j]=a[i][j]+b[i][j]
            {
                tempj = a.data[p1].j;
                tempv = a.data[p1].val + b.data[p2].val;
                ++p1;
                ++p2;
            }
            if (tempv) // c[i][j]!=0
            {
                if (c.tot + 1 >= c.sizeOfMatrix)
                    expandMatrix(&c);
                c.data[++c.tot] = (Tuple) {i, tempj, tempv};
            }
        }
    }
    c.pos[c.n + 1] = c.tot + 1;
    // for (int i = 1; i <= c.tot; ++i)
    // printf("%d %d %d\n", c.data[i].i, c.data[i].j, c.data[i].val);
    return c;
}

// 把两个矩阵相乘
Matrix mulMatrix(Matrix a, Matrix b)
{
    if (a.m != b.n) // 两个矩阵不可相乘
        exit(1);
    Matrix c;
    initMatrix(&c, a.n, b.m);
    if (!a.tot || !b.tot) // 如果有一个矩阵为空，则返回空矩阵
        return c;
    long long * temp;
    if (!(temp = (long long *) calloc(b.m + 1, sizeof(long long)))) // 临时数组，用于记录c矩阵中每一行的结果
        exit(1);
    for (int i = 1; i <= a.n; ++i)
    {
        memset(temp, 0, (b.m + 1) * sizeof(long long));
        c.pos[i] = c.tot + 1;
        for (int p = a.pos[i]; p < a.pos[i + 1]; ++p) // 枚举a矩阵第i行的非零元素
        {
            int k = a.data[p].j; // a矩阵的该非零元素为a[i][k]
            for (int q = b.pos[k]; q < b.pos[k + 1]; ++q) // 枚举b矩阵第k行的非零元素
            {
                int j = b.data[q].j; // b矩阵的该非零元素为b[k][j]
                temp[j] += a.data[p].val * b.data[q].val; // c[i][j]+=a[i][k]*b[k][j]
            }
        }
        for (int j = 1; j <= b.m; ++j)
        {
            if (!temp[j]) // c[i][j]==0
                continue;
            if (c.tot + 1 >= c.sizeOfMatrix)
                expandMatrix(&c);
            c.data[++c.tot] = (Tuple) {i, j, temp[j]};
        }
    }
    c.pos[c.n + 1] = c.tot + 1;
    free(temp);
    return c;
}

// 把由三元组表存储的矩阵转化为由一维数组存储的矩阵
void matrix2Array(Matrix a, long long val[])
{
    memset(val, 0, sizeof(long long) * (a.n * a.m + 1));
    for (int i = 1; i <= a.tot; ++i)
        val[(a.data[i].i - 1) * a.m + a.data[i].j] = a.data[i].val; // a[i][j]存储在val[(i-1]*m+j]中
}

// 释放矩阵空间
void destroyMatrix(Matrix * a)
{
    free(a->pos);
    free(a->data);
    a->n = a->m = a->tot = a->sizeOfMatrix = 0;
}
