//
// Created by xqmmcqs on 2021/3/12.
//

#include "graph.h"

#include <queue>
#include <memory>
#include <algorithm>
#include <random>

using namespace std;

const int INF = 0x3f3f3f3f;

void Graph::loadMap(int nodeNum, const vector<MapNode> &nodeData, const vector<EdgeData> &edgeData)
{
    vertex_count = nodeNum;
    nodes = nodeData;
    edges.resize(nodeNum);
    for (const auto &i : edgeData)
        edges[i.st].push_back({i.id, i.st, i.ed, i.length, 0, i.level});
}

void Graph::loadPort(const vector<PortNode> &portData, double _scale)
{
    ports = portData;
    scale = _scale;
}

void Graph::setSpeed(int curTime)
{
    if (lastTime > curTime)lastTime -= 24 * 60 * 60;
    if (curTime - lastTime <= 15 * 60)return;
    normal_distribution<double> u{340, 50};
    default_random_engine e(time(nullptr));
    for (auto &i:nodes)
        i.speed = u(e);
    for (auto &i:edges)
        for (auto &j:i)
            j.speed = (nodes[j.st].speed + nodes[j.ed].speed) / 2;
    lastTime = curTime;
}

int Graph::dijkstra(int st, int ed, Path &solution, bool isRandom, bool isBike) const
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    vector<int> dis(vertex_count);
    vector<int> lastNode(vertex_count);
    for (int i = 0; i < vertex_count; ++i)
    {
        lastNode[i] = -1;
        dis[i] = INF;
    }
    dis[st] = 0;
    q.push(make_pair(0, st));
    while (!q.empty())
    {
        auto u = q.top().second;
        if (u == ed)break;
        q.pop();
        for (const auto &edge:edges[u])
        {
            if (isBike && !edge.level) continue; // 只能走自行车道
            if (dis[edge.ed] > dis[u] + edge.length / ((isRandom ? edge.speed : avgSpeed) / (isBike ? 1 : 3)))
            {
                dis[edge.ed] = dis[u] + edge.length / ((isRandom ? edge.speed : avgSpeed) / (isBike ? 1 : 3));
                lastNode[edge.ed] = edge.st;
                q.push(make_pair(dis[edge.ed], edge.ed));
            }
        }
    }
    int ret = dis[ed];
    solution.clear();
    solution.push_back(&nodes[ed]);
    while (~lastNode[ed])
    {
        solution.push_back(&nodes[lastNode[ed]]);
        ed = lastNode[ed];
    }
    reverse(solution.begin(), solution.end());
    return ret;
}
