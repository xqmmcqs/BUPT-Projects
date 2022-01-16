#include <bits/stdc++.h>
using namespace std;
inline int read()
{
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9')
    {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9')
    {
        x = (x << 1) + (x << 3) + ch - '0';
        ch = getchar();
    }
    return x * f;
}

const int MAXN = 1e3 + 10, INF = 0x3f3f3f3f;
int n, m, dis[MAXN][MAXN];

void floyd()
{
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (dis[i][k] + dis[k][j] < dis[i][j])
                    dis[i][j] = dis[i][k] + dis[k][j];
}

int main()
{
    n = read();
    m = read();
    memset(dis, INF, sizeof(dis));
    for (int i = 1; i <= n; ++i)
        dis[i][i] = 0;
    for (int i = 1; i <= m; ++i)
    {
        int x = read(), y = read(), z = read();
        dis[x][y] = min(dis[x][y], z);
    }
    floyd();
    int ans1 = 0, ans2, ans3;
    for (int j = 1; j <= n; ++j)
        for (int i = 1; i <= n; ++i)
            if (dis[i][j] != INF && dis[i][j] > ans1)
            {
                ans1 = dis[i][j];
                ans2 = i;
                ans3 = j;
            }
    printf("%d %d %d\n", ans1, ans2, ans3);
    return 0;
}
