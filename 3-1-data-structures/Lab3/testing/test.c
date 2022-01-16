#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long a[5010][5010], b[5010][5010];
int na, ma, nb, mb;

int main()
{
    scanf("%d%d", &na, &ma);
    if (na <= 0 || ma <= 0)
    {
        puts("Please check your input.");
        exit(1);
    }
    for (int i = 1; i <= na; ++i)
        for (int j = 1; j <= ma; ++j)
            scanf("%lld", &a[i][j]);
    scanf("%d%d", &nb, &mb);
    if (nb <= 0 || mb <= 0)
    {
        puts("Please check your input.");
        exit(1);
    }
    for (int i = 1; i <= nb; ++i)
        for (int j = 1; j <= mb; ++j)
            scanf("%lld", &b[i][j]);
    if (na == nb && ma == mb)
    {
        for (int i = 1; i <= na; ++i)
        {
            for (int j = 1; j <= ma; ++j)
                printf("%lld ", a[i][j] + b[i][j]);
            puts("");
        }
    }
    else
    {
        printf("Cannot add matrix A and B");
        if (na != nb)
            printf(", An != Bn");
        if (ma != mb)
            printf(", Am != Bm");
        puts(".");
    }
    if (ma == nb)
    {
        for (int i = 1; i <= na; ++i)
        {
            for (int j = 1; j <= mb; ++j)
            {
                long long c = 0;
                for (int k = 1; k <= nb; ++k)
                {
                    c += a[i][k] * b[k][j];
                }
                printf("%lld ", c);
            }
            puts("");
        }
    }
    else
        puts("Cannot multiply matrix A and B, Am != Bn.");
    return 0;
}
