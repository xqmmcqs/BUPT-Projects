//
// Created by xqmmcqs on 2021/3/12.
//

#include <fstream>
#include <memory>
#include <list>

#include <restbed>
#include <json/json.h>

#include "graph.h"

using namespace std;
using namespace restbed;

int graphNum;
vector<Graph> graph;
unordered_map<unsigned long long, int> id2index;
vector<int> timeTable;

/**
 * @brief 加载地图
 *
 * @param area 子图ID
 * @param fileName 存储地图信息的文件名
 */
static void setMap(int area, const string &fileName)
{
    ifstream file(fileName);
    Json::Value root;
    file >> root;
    
    vector<MapNode> nodeData;
    vector<Graph::EdgeData> edgeData;
    
    const Json::Value nodeJson = root["elements"]["nodes"];
    // 加载节点信息
    for (const auto &i : nodeJson)
    {
        string name = i["data"]["title"].asString();
        int posX = i["position"]["x"].asInt(), posY = i["position"]["y"].asInt();
        auto id = i["data"]["id"].asUInt64() << 8 | area;
        
        nodeData.push_back({id, area, name, {posX, posY}});
        id2index[id] = nodeData.size() - 1;
    }
    
    const Json::Value edgeJson = root["elements"]["edges"];
    // 加载边信息
    for (const auto &i: edgeJson)
    {
        int u = id2index[i["data"]["source"].asUInt64() << 8 | area], v = id2index[i["data"]["target"].asUInt64() << 8 |
                area];
        auto id = i["data"]["id"].asUInt64() << 8 | area;
        int level = i["data"]["level"].asInt();
        int val = graph[area].getDis(nodeData[u].position, nodeData[v].position);
        edgeData.push_back({id, u, v, val, level});
        edgeData.push_back({id, v, u, val, level});
    }
    
    auto n = nodeData.size();
    graph[area].loadMap(n, nodeData, edgeData);
}

/**
 * @brief 加载地图信息
 *
 * @param fileName 地图信息文件名
 * @param mapFiles 子图ID及存储各子图信息的文件名
 */
static void setGraphNum(const string &fileName, vector<pair<int, string>> &mapFiles)
{
    ifstream file(fileName);
    Json::Value root;
    file >> root;
    
    const Json::Value portJson = root["maps"];
    // 从maps中读取最大ID
    for (const auto &i : portJson)
    {
        graphNum = max(graphNum, i["id"].asInt() + 1);
        mapFiles.emplace_back(i["id"].asInt(), i["path"].asString());
    }
}

/**
 * @brief 加载子图间通道信息
 *
 * @param fileName 地图信息文件名
 */
static void setPort(const string &fileName)
{
    ifstream file(fileName);
    Json::Value root;
    file >> root;
    
    vector<vector<PortNode>> portData(graphNum);
    vector<double> scale(graphNum);
    
    const Json::Value mapJson = root["maps"];
    for (const auto &i:mapJson)
        scale[i["id"].asInt()] = i["scale"].asDouble();
    
    const Json::Value portJson = root["ports"];
    for (const auto &i : portJson)
    {
        auto id = i["id"].asUInt64();
        auto level = i["level"].asInt();
        auto stMapId = i["sourceMapId"].asUInt64(), edMapId = i["targetMapId"].asUInt64();
        auto stNodeId = i["sourceNodeId"].asUInt64() << 8 | stMapId, edNodeId =
                i["targetNodeId"].asUInt64() << 8 | edMapId;
        
        portData[stMapId].push_back({id, level, stMapId, edMapId, stNodeId, edNodeId});
    }
    
    for (int i = 0; i < graphNum; ++i)
        graph[i].loadPort(portData[i], scale[i]);
}

void getMap(const shared_ptr<Session> &session)
{
    Json::Value root;
    const auto request = session->get_request();
    int area = stoi(request->get_path_parameter("id"));
    
    auto nodes = graph[area].getNodes();
    Json::Value nodeJson;
    for (const auto &i : nodes)
    {
        Json::Value node;
        node["data"]["title"] = i.name;
        node["position"]["x"] = i.position.x;
        node["position"]["y"] = i.position.y;
        node["data"]["id"] = static_cast<Json::Value::UInt64>(i.id >> 8);
        nodeJson.append(node);
    }
    root["elements"]["nodes"] = nodeJson;
    
    Json::Value edgeJson;
    auto edges = graph[area].getEdges();
    for (const auto &i: edges)
        for (const auto &j: i)
        {
            Json::Value edge;
            edge["data"]["source"] = j.st >> 8;
            edge["data"]["target"] = j.ed >> 8;
            edge["data"]["id"] = static_cast<Json::Value::UInt64>(j.id >> 8);
            edge["data"]["level"] = j.level;
            edgeJson.append(edge);
        }
    root["elements"]["edges"] = edgeJson;
    
    ostringstream ss;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(root, &ss);
    session->close(OK, ss.str(), {{"Content-Length", to_string(ss.str().size())}});
}

void getMapMeta(const shared_ptr<Session> &session)
{
    Json::Value root;
    Json::Value maps, ports;
    for (int i = 0; i < graphNum; ++i)
    {
        maps.append(i);
        for (const auto &j: graph[i].getPorts())
        {
            Json::Value port;
            port["id"] = static_cast<Json::Value::UInt64>(j.id);
            port["level"] = j.level;
            port["sourceMapId"] = static_cast<Json::Value::UInt64>(j.stMapId);
            port["targetMapId"] = static_cast<Json::Value::UInt64>(j.edMapId);
            port["sourceNodeId"] = static_cast<Json::Value::UInt64>(j.stNodeId);
            port["targetNodeId"] = static_cast<Json::Value::UInt64>(j.edNodeId);
            ports.append(port);
        }
    }
    root["maps"] = maps;
    root["ports"] = ports;
    
    ostringstream ss;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(root, &ss);
    session->close(OK, ss.str(), {{"Content-Length", to_string(ss.str().size())}});
}

/**
 * @brief 获取从当前区域到目标区域的路径
 *
 * @param area 当前区域
 * @param lastArea 上一个区域
 * @param crossPort 经过的图间通道
 * @param edArea 目标区域
 * @return 如果存在当前区域到目标区域的路径，返回true
 */
static bool getCrossPath(unsigned long long area, unsigned long long lastArea, vector<const PortNode *> &crossPort,
                         unsigned long long edArea)
{
    if (area == edArea)return true;
    for (const auto &i:graph[area].getPorts())
    {
        if (i.edMapId == lastArea)continue;
        crossPort.push_back(&i);
        if (getCrossPath(i.edMapId, area, crossPort, edArea))return true;
        crossPort.pop_back();
    }
    return false;
}

/**
 * @brief 将以四位数小时分钟表示的时间转化成以秒表示的时间
 *
 * @param time 以四位数小时分钟表示的时间
 * @return 以秒表示的时间
 */
static inline int time2sec(int time)
{
    return time / 100 * 3600 + time % 100 * 60;
}

/**
 * @brief 获取经过图间通道需要的时间
 *
 * @param curTime 出发时刻
 * @param crossType 跨越通道的类型
 * @return 需要的时间
 */
static int getCrossTime(int curTime, int crossType)
{
    int ret;
    if (crossType == 2) // 出租车
    {
        if (time2sec(0600) <= curTime && curTime <= time2sec(1000) ||
            time2sec(1700) <= curTime && curTime <= time2sec(2100)) // 高峰期
            ret = 50 * 60;
        else ret = 30 * 60;
    }
    else if (crossType == 4) // 校车
    {
        ret = time2sec(2400);
        for (const auto &i: timeTable)
            ret = min(ret, i > curTime ? i - curTime : i + time2sec(2400) - curTime);
        if (time2sec(0600) <= curTime && curTime <= time2sec(1000) ||
            time2sec(1700) <= curTime && curTime <= time2sec(2100)) // 高峰期
            ret += 60 * 60;
        else ret += 40 * 60;
    }
    else // 公交车
    {
        if (time2sec(0500) >= curTime)
            ret = time2sec(0500) - curTime + 30 * 60;
        else if (time2sec(2330) <= curTime)
            ret = time2sec(0500) + time2sec(2400) - curTime + 30 * 60;
        else ret = 0;
        ret += (curTime / 600 + 1) * 600 - curTime;
        ret += 85 * 60;
    }
    return ret;
}

/**
 * @brief 获取一条路径
 *
 * @param st 起点
 * @param ed 终点
 * @param stArea 起点所在子图
 * @param edArea 终点所在子图
 * @param type 最短路径/最短时间
 * @param isBike 是否骑自行车
 * @param time 出发时间
 * @param crossType 跨越子图的方式
 * @param crossTime 跨越子图的时间
 * @param path 路径
 * @param totalTime 总时间
 * @param curTime 当前时间
 * @return 如果存在从起点到终点的路径，则返回true
 */
bool
getOnePath(unsigned long long st, unsigned long long ed, unsigned long long stArea, unsigned long long edArea, int type,
           int isBike, int time, int crossType, list<int> &crossTime, Path &path, int &totalTime, int &curTime)
{
    vector<const PortNode *> crossPort;
    getCrossPath(stArea, -1, crossPort, edArea);
    Path areaPath;
    auto area = stArea;
    auto curSt = st; // 当前图的起点
    
    for (const auto &i: crossPort)
    {
        graph[area].setSpeed(time + curTime);
        curTime = graph[area].dijkstra(id2index[curSt], id2index[i->stNodeId], areaPath, type, isBike);
        if (curTime > 1000000) return false;
        totalTime += curTime;
        path.insert(path.end(), areaPath.begin(), areaPath.end());
        
        if (i->level < 16 && (i->level & crossType) == 0)return false;
        path.push_back(i);
        if (i->level >= 16)
            crossTime.push_back(0);
        else
        {
            int thisTime = getCrossTime((time + totalTime) % time2sec(2400), crossType);
            crossTime.push_back(thisTime);
            totalTime += thisTime;
        }
        area = i->edMapId;
        curSt = i->edNodeId;
    }
    graph[area].setSpeed(time + curTime);
    curTime = graph[area].dijkstra(id2index[curSt], id2index[ed], areaPath, type, isBike);
    if (curTime > 1000000)return false;
    totalTime += curTime;
    path.insert(path.end(), areaPath.begin(), areaPath.end());
    return true;
}

void getPath(const shared_ptr<Session> &session)
{
    const auto request = session->get_request();
    auto stArea = stoull(request->get_query_parameter("source_map_id"));
    auto edArea = stoull(request->get_query_parameter("target_map_id"));
    auto st = stoull(request->get_query_parameter("source_node_id")) << 8 | stArea;
    auto ed = stoull(request->get_query_parameter("target_node_id")) << 8 | edArea;
    int type = stoi(request->get_query_parameter("path_type"));
    int isBike = stoi(request->get_query_parameter("prefer_transport_type"));
    int time = stoi(request->get_query_parameter("depart_at"));
    int crossType = stoi(request->get_query_parameter("cross_type"));
    
    Path path;
    int totalTime = 0, curTime = 0;
    list<int> crossTime;
    
    if (!getOnePath(st, ed, stArea, edArea, type, isBike, time, crossType, crossTime, path, totalTime, curTime))
    {
        session->close(BAD_REQUEST);
        return;
    }
    
    Json::Value root;
    Json::Value pathJson;
    for (const auto &i:path)
    {
        Json::Value nodeJson;
        if (i->getType())
        {
            auto node = dynamic_cast<const MapNode *>(i);
            nodeJson["type"] = "node";
            nodeJson["id"] = static_cast<Json::Value::UInt64>(node->id) >> 8;
            nodeJson["mapId"] = node->mapId;
        }
        else
        {
            auto node = dynamic_cast<const PortNode *>(i);
            nodeJson["type"] = "port";
            nodeJson["id"] = static_cast<Json::Value::UInt64>(node->id);
            nodeJson["time"] = crossTime.front();
            if (node->level >= 16)nodeJson["level"] = node->level;
            else nodeJson["level"] = crossType;
            crossTime.pop_front();
        }
        pathJson.append(nodeJson);
    }
    root["path"] = pathJson;
    root["time"] = totalTime;
    
    ostringstream ss;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(root, &ss);
    session->close(OK, ss.str(), {{"Content-Length", to_string(ss.str().size())}});
}

static void getPathsCb(const shared_ptr<Session> &session, const Bytes &payload)
{
    bool res;
    JSONCPP_STRING errs;
    Json::Value req;
    Json::CharReaderBuilder readerBuilder;
    unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    res = jsonReader->parse((char *) payload.data(), (char *) payload.data() + payload.size(), &req, &errs);
    if (!res)
    {
        session->close(BAD_REQUEST);
        return;
    }
    
    auto type = req["path_type"].asInt();
    auto isBike = req["prefer_transport_type"].asInt();
    auto time = req["depart_at"].asInt();
    auto crossType = req["cross_type"].asInt();
    
    Path path;
    int totalTime = 0, curTime = 0;
    list<int> crossTime;
    
    auto nodes = req["nodes"];
    vector<unsigned long long> nodeId;
    vector<unsigned long long> mapId;
    for (const auto &i:nodes)
    {
        mapId.push_back(i["mapId"].asUInt64());
        nodeId.push_back(i["nodeId"].asUInt64() << 8 | i["mapId"].asUInt64());
    }
    for (auto i = 0; i < nodeId.size() - 1; ++i)
    {
        auto st = nodeId[i];
        auto ed = nodeId[i + 1];
        auto stArea = mapId[i];
        auto edArea = mapId[i + 1];
        if (!getOnePath(st, ed, stArea, edArea, type, isBike, time, crossType, crossTime, path, totalTime, curTime))
        {
            session->close(BAD_REQUEST);
            return;
        }
    }
    
    Json::Value root;
    Json::Value pathJson;
    for (const auto &i:path)
    {
        Json::Value nodeJson;
        if (i->getType())
        {
            auto node = dynamic_cast<const MapNode *>(i);
            nodeJson["type"] = "node";
            nodeJson["id"] = static_cast<Json::Value::UInt64>(node->id) >> 8;
            nodeJson["mapId"] = node->mapId;
        }
        else
        {
            auto node = dynamic_cast<const PortNode *>(i);
            nodeJson["type"] = "port";
            nodeJson["id"] = static_cast<Json::Value::UInt64>(node->id);
            nodeJson["time"] = crossTime.front();
            if (node->level >= 16)nodeJson["level"] = node->level;
            else nodeJson["level"] = crossType;
            crossTime.pop_front();
        }
        pathJson.append(nodeJson);
    }
    root["path"] = pathJson;
    root["time"] = totalTime;
    
    ostringstream ss;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(root, &ss);
    session->close(OK, ss.str(), {{"Content-Length", to_string(ss.str().size())}});
}

void getPaths(const shared_ptr<Session> &session)
{
    const auto request = session->get_request();
    size_t content_length = request->get_header("Content-Length", 0);
    session->fetch(content_length, getPathsCb);
}

void getSpeed(const shared_ptr<Session> &session)
{
    const auto request = session->get_request();
    auto area = stoull(request->get_query_parameter("map_id"));
    auto time = stoi(request->get_query_parameter("depart_time"));
    auto type = stoi(request->get_query_parameter("type"));
    
    graph[area].setSpeed(time);
    
    map<unsigned long long, int> speed;
    int maxSpeed = 0, minSpeed = 10000;
    if (type) // 边速度
    {
        for (const auto &i:graph[area].getEdges())
            for (const auto &j:i)
            {
                speed[j.id] = j.speed;
                maxSpeed = max(j.speed, maxSpeed);
                minSpeed = min(j.speed, minSpeed);
            }
    }
    else // 点速度
    {
        for (const auto &i:graph[area].getNodes())
        {
            speed[i.id] = i.speed;
            maxSpeed = max(i.speed, maxSpeed);
            minSpeed = min(i.speed, minSpeed);
        }
    }
    Json::Value root;
    for (const auto &i: speed)
    {
        Json::Value object;
        object["id"] = static_cast<Json::Value::UInt64>(i.first) >> 8;
        object["speed"] = static_cast<double>(i.second - minSpeed) / (maxSpeed - minSpeed);
        root.append(object);
    }
    
    ostringstream ss;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(root, &ss);
    session->close(OK, ss.str(), {{"Content-Length", to_string(ss.str().size())}});
}

void optionsHandler(const shared_ptr<Session> &session)
{
    session->close(OK);
}

int main()
{
    ifstream is("timetable.txt");
    int time;
    while (is >> time)
        timeTable.push_back(time2sec(time));
    
    vector<pair<int, string>> mapFiles;
    setGraphNum("port.json", mapFiles);
    graph.resize(graphNum);
    setPort("port.json");
    for (const auto &i:mapFiles)
        setMap(i.first, i.second);
    
    auto getmapmeta = make_shared<Resource>();
    getmapmeta->set_path("/map_meta");
    getmapmeta->set_method_handler("GET", getMapMeta);
    getmapmeta->set_method_handler("OPTIONS", optionsHandler);
    
    auto getmap = make_shared<Resource>();
    getmap->set_path("/map/{id: [0-9]*}");
    getmap->set_method_handler("GET", getMap);
    getmap->set_method_handler("OPTIONS", optionsHandler);
    
    auto getpath = make_shared<Resource>();
    getpath->set_path("/path");
    getpath->set_method_handler("GET", getPath);
    getpath->set_method_handler("OPTIONS", optionsHandler);
    
    auto getpaths = make_shared<Resource>();
    getpaths->set_path("/paths");
    getpaths->set_method_handler("POST", getPaths);
    getpaths->set_method_handler("OPTIONS", optionsHandler);
    
    auto getspeed = make_shared<Resource>();
    getspeed->set_path("/speed");
    getspeed->set_method_handler("GET", getSpeed);
    getspeed->set_method_handler("OPTIONS", optionsHandler);
    
    auto settings = make_shared<Settings>();
    settings->set_port(1926);
    settings->set_default_header("Connection", "close");
    settings->set_default_header("Access-Control-Allow-Origin", "*");
    settings->set_default_header("Access-Control-Allow-Methods", "*");
    settings->set_default_header("Access-Control-Allow-Headers", "*");
    
    Service service;
    service.publish(getmapmeta);
    service.publish(getmap);
    service.publish(getpath);
    service.publish(getpaths);
    service.publish(getspeed);
    service.start(settings);
    return EXIT_SUCCESS;
}
