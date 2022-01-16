//
// Created by xqmmcqs on 2020/11/27.
//

#include <stdio.h>
#include "graph.h"

int main()
{
    scanf("%d%d", &n, &m);
    if (m <= 0 || n <= 1) // 边不足或者点不足，无解
    {
        puts("No solution.");
        return 0;
    }
    initGraph(); // 初始化图
    for (int i = 1; i <= m; ++i)
    {
        int u, v, val;
        scanf("%d%d%d", &u, &v, &val);
        if (u < 1 || u > n || v < 1 || v > n || val <= 0 || u == v) // 边权信息有误
        {
            puts("Invalid input.");
            return 0;
        }
        u--;
        v--;
        addEdge(u, v, val);
    }
    if (topologicalSort())
    {
        puts("Invalid input."); // 不是有向无环图
        return 0;
    }
    int ans1 = -1, ans2; // ans1是最长距离，ans2是该距离的终点
    for (int i = 0; i < n; ++i) // 找到最长距离以及终点
        if (dist[i] > ans1)
        {
            ans1 = dist[i];
            ans2 = i;
        }
//    printf("%d\n", ans1);
    printf("%d %d %d\n", ans1, start[ans2] + 1, ans2 + 1);
    destroyGraph();
    return 0;
}
