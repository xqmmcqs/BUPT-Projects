//
// Created by xqmmcqs on 2020/11/30.
//

#include <string.h>
#include <stdlib.h>
#include "graph.h"

int n, m;
int * ind, * dist, * start;
Edge ** firstEdge;
const int INF = 0x7f7f7f7f;

// 初始化有向无环图
void initGraph()
{
    if (!(firstEdge = (Edge **) calloc(n, sizeof(Edge *))))
        exit(1);
    if (!(dist = (int *) calloc(n, sizeof(int *))))
        exit(1);
    if (!(ind = (int *) calloc(n, sizeof(int))))
        exit(1);
    if (!(start = (int *) calloc(n, sizeof(int))))
        exit(1);
}

// 向图中加入弧
void addEdge(int u, int v, int val)
{
    Edge * temp;
    if (!(temp = (Edge *) calloc(1, sizeof(Edge))))
        exit(1);
    temp->val = val;
    temp->v = v;
    temp->next = firstEdge[u];
    firstEdge[u] = temp;
    ++ind[v];
}

// 有向无环图的拓扑排序，求出dist和startOfPoint数组的值
bool topologicalSort()
{
    int * tempdist, * order, head = 1, tail = 0; // tempdist用于临时存放每个源点到各个顶点的距离，order为拓扑序，head和tail是队列指针
    if (!(tempdist = (int *) calloc(n + 1, sizeof(int))))
        exit(1);
    if (!(order = (int *) calloc(n + 1, sizeof(int))))
        exit(1);
    for (int i = 0; i < n; ++i)
        if (!ind[i])
            order[++tail] = i;
    while (head <= tail) // 拓扑排序
    {
        int u = order[head++];
        for (Edge * i = firstEdge[u]; i; i = i->next)
        {
            --ind[i->v];
            if (!ind[i->v])
                order[++tail] = i->v;
        }
    }
    for (int i = 0; i < n; ++i) // 不是有向无环图
        if (ind[i])
            return 1;
    for (int i = 1; i <= n; ++i)
    {
        memset(tempdist, INF, n * sizeof(int));
        tempdist[order[i]] = 0; // order[i]为源点
        for (int j = i; j <= n; ++j) // 依照拓扑序遍历剩余的点
        {
            int u = order[j];
            if (tempdist[u] != INF && tempdist[u] > dist[u]) // 从order[i]到u的距离比原先以u为终点的距离大
            {
                dist[u] = tempdist[u];
                start[u] = order[i];
            }
            for (Edge * i = firstEdge[u]; i; i = i->next)
                if (tempdist[u] + i->val < tempdist[i->v])
                    tempdist[i->v] = tempdist[u] + i->val;
        }
    }
    free(tempdist);
    free(order);
    return 0;
}

// 释放有向无环图的空间
void destroyGraph()
{
    for (int u = 0; u < n; ++u)
        for (Edge * i = firstEdge[u], * t; i; t = i, i = i->next, free(t));
    free(firstEdge);
    free(dist);
    free(ind);
    free(start);
}
