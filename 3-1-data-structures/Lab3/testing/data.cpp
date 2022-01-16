#include <bits/stdc++.h>
using namespace std;

const int SIZE = 1e3, PERCENT = 50;

int main()
{
    srand(time(0));
    int n = rand() % SIZE + 1;
    printf("%d %d\n", n, n);
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
            printf("%d ", rand() % 100 > PERCENT ? 0 : rand() % 1000 + 1);
        puts("");
    }
    printf("%d %d\n", n, n);
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
            printf("%d ", rand() % 100 > PERCENT ? 0 : rand() % 1000 + 1);
        puts("");
    }
    return 0;
}
