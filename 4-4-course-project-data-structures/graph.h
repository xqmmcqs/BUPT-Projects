//
// Created by xqmmcqs on 2021/3/12.
//

#ifndef DS_DESIGN_GRAPH_H
#define DS_DESIGN_GRAPH_H

#include <vector>
#include <string>
#include <tuple>
#include <unordered_map>

#include<cmath>

struct Vector2D
{
    int x;
    int y;
    
    Vector2D(int _x, int _y) : x(_x), y(_y)
    {}
    
    Vector2D operator+(Vector2D b) const
    {
        return {x + b.x, y + b.y};
    }
};

struct Node
{
    unsigned long long id;
    
    Node(unsigned long long _id) : id(_id)
    {}
    
    virtual bool getType() const = 0;
};

struct MapNode : public Node
{
    int mapId;
    std::string name;
    Vector2D position;
    int speed = 0;
    
    MapNode(unsigned long long _id, int _mapId, std::string _name, Vector2D _position) :
            Node(_id), mapId(_mapId), name(std::move(_name)), position(_position)
    {}
    
    bool getType() const override
    { return true; }
};

struct PortNode : public Node
{
    int level;
    unsigned long long stMapId;
    unsigned long long edMapId;
    unsigned long long stNodeId;
    unsigned long long edNodeId;
    
    PortNode(unsigned long long _id, int _level, unsigned long long _stMapId, unsigned long long _edMapId,
             unsigned long long _stNodeId, unsigned long long _edNodeId) :
            Node(_id), level(_level), stMapId(_stMapId), edMapId(_edMapId), stNodeId(_stNodeId), edNodeId(_edNodeId)
    {}
    
    bool getType() const override
    { return false; }
};

struct EdgeNode
{
    unsigned long long id;
    int st;
    int ed;
    int length;
    int speed;
    int level;
};

typedef std::vector<const Node *> Path;

class Graph
{
private:
    int vertex_count = 0;
    std::vector<MapNode> nodes;
    std::vector<std::vector<EdgeNode>> edges;
    std::vector<PortNode> ports;
    double scale;
    int lastTime = -10000; ///< 上一次随机速度的时间
    int avgSpeed = 340;

public:
    struct EdgeData
    {
        unsigned long long id;
        int st;
        int ed;
        int length;
        int level;
    };
    
    Graph() = default;
    
    void loadMap(int nodeNum, const std::vector<MapNode> &nodeData, const std::vector<EdgeData> &edgeData);
    
    void loadPort(const std::vector<PortNode> &portData, double scale);
    
    const std::vector<MapNode> &getNodes() const
    { return nodes; }
    
    const std::vector<std::vector<EdgeNode>> &getEdges() const
    { return edges; }
    
    const std::vector<PortNode> &getPorts() const
    { return ports; }
    
    ~Graph() = default;
    
    int getDis(const Vector2D &u, const Vector2D &v) const
    {
        return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y)) * 100 * scale;
    }
    
    void setSpeed(int time);
    
    int dijkstra(int st, int ed, Path &solution, bool isRandom, bool isBike) const;
};

#endif //DS_DESIGN_GRAPH_H
