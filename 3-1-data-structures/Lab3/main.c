//
// Created by xqmmcqs on 2020/10/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

long long val[5010 * 5010];

int main()
{
    int n, m, cnt = 0;
    scanf("%d%d", &n, &m);
    if (n <= 0 || m <= 0)
    {
        puts("Please check your input.");
        exit(1);
    }
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            scanf("%lld", &val[++cnt]); // 输入A矩阵
    Matrix a = array2Matrix(n, m, val);
    cnt = 0;
    scanf("%d%d", &n, &m);
    if (n <= 0 || m <= 0)
    {
        puts("Please check your input.");
        exit(1);
    }
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            scanf("%lld", &val[++cnt]); // 输入B矩阵
    Matrix b = array2Matrix(n, m, val);
    if (a.n == b.n && a.m == b.m) // 如果矩阵能相加
    {
        Matrix c = addMatrix(a, b);
        matrix2Array(c, val);
        cnt = 0;
        for (int i = 1; i <= c.n; ++i)
        {
            for (int j = 1; j <= c.m; ++j)
                printf("%lld ", val[++cnt]);
            putchar('\n');
        }
        destroyMatrix(&c);
    }
    else
    {
        printf("Cannot add matrix A and B");
        if (a.n != b.n)
            printf(", An != Bn");
        if (a.m != b.m)
            printf(", Am != Bm");
        puts(".");
    }
    if (a.m == b.n)
    {
        Matrix c = mulMatrix(a, b);
        matrix2Array(c, val);
        cnt = 0;
        for (int i = 1; i <= c.n; ++i)
        {
            for (int j = 1; j <= c.m; ++j)
                printf("%lld ", val[++cnt]);
            putchar('\n');
        }
        destroyMatrix(&c);
    }
    else
        puts("Cannot multiply matrix A and B, Am != Bn.");
    destroyMatrix(&a);
    destroyMatrix(&b);
    return 0;
}