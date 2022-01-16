//
// Created by xqmmcqs on 2020/11/30.
//

#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H
#include <stdbool.h>

typedef struct edge
{
    int v, val;
    struct edge * next;
} Edge;

extern int n, m; // n为点数，m为边数
extern int * ind; // 各个顶点的入度
extern int * dist; // 源点至各个顶点的最短距离
extern int * start; // 距离这个点最远的点的编号
extern Edge ** firstEdge;

/*
 * 操作：初始化有向无环图
 * 后件：初始化firstEdge、ind、dist、start数组
 */
void initGraph();

/*
 * 操作：向图中加入弧
 * 前件：u,v是弧尾、弧头的编号，val是弧的权值
 * 后件：图中加入这条边
 */
void addEdge(int u, int v, int val);

/*
 * 操作：有向无环图的拓扑排序，求出dist和start数组的值
 * 前件：firstEdge中存储有向无环图的信息
 * 后件：如果有向无环图合法，则返回0，并且dist中存储源点至各个点的最短距离，start中记录最远的点的编号；否则函数返回1
 */
bool topologicalSort();

/*
 * 操作：释放有向无环图的空间
 * 后件：释放各条边的空间，以及firstEdge、ind、dist、start数组的空间
 */
void destroyGraph();

#endif //LAB5_GRAPH_H