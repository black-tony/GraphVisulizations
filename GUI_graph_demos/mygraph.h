#ifndef MYGRAPH_H
#define MYGRAPH_H

#include <QObject>
//#include "linklist.h"
template<typename T>
struct Edge{
    int v;
    T val;
    Edge(int v = -1, const T &val = -1)
    {
        if(v == -1)
            return;
        this->v = v;
        this->val = val;
    }
};
template<typename T>
class MyGraphMap
{
public:
    explicit MyGraphMap()
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
        NodeNum = 0;
    }
    explicit MyGraphMap(int nodeNum)
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
        setGraphLimit(nodeNum);
    }
    explicit MyGraphMap(const MyGraphMap<T> &g)
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
        setInvalidEdge(g.NOT_A_EDGE);
        setGraphLimit(g.getNodeNum());
        this->graphMap = g.graphMap;
    }
    MyGraphMap<T>& operator=(const MyGraphMap<T> &oth)
    {
        setGraphLimit(oth.getNodeNum());
        setInvalidEdge(oth.NOT_A_EDGE);
        graphMap = oth.graphMap;
        return *this;
    }
    QList<T> &operator[](qsizetype index)
    {
        return this->graphMap[index];
    }
    const QList<T> &operator[](qsizetype index) const
    {
        return this->graphMap[index];
    }
    int getEdgeNum() const
    {
        int edgeNum = 0;
        for(const auto &i : graphMap)
        {
            for(const auto&j : i)
            {
                if(j != NOT_A_EDGE)
                    edgeNum++;
            }
        }
        return edgeNum;
    }
    int getNodeNum() const
    {
        return NodeNum;
    }
    void setGraphLimit(int NodeNum = 0)
    {
        this->NodeNum = NodeNum;
        graphMap.resize(NodeNum, QList<T>(NodeNum, NOT_A_EDGE));
    }
    void setInvalidEdge(const T& val)
    {
        _NOT_A_EDGE = val;
    }
    const T& NOT_A_EDGE;
private:
    T _NOT_A_EDGE;
    int NodeNum;
    QList<QList<T>> graphMap;
};



template<typename T>
class MyGraphList
{
public:
    explicit MyGraphList(int nodeNum)
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
//        NOT_A_EDGE = _NOT_A_EDGE;
        setNodeNum(nodeNum);
    }
    explicit MyGraphList()
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
//        NOT_A_EDGE = _NOT_A_EDGE;
        nodeNum = 0;
    }
    MyGraphList(const MyGraphList<T> &g)
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
        setInvalidEdge(g.NOT_A_EDGE);
        setNodeNum(g.getNodeNum());
        this->edge = g.edge;
    }
    explicit MyGraphList(const MyGraphMap<T> &graph)
        :NOT_A_EDGE(_NOT_A_EDGE)
    {
        setNodeNum(graph.getNodeNum());
        setInvalidEdge(graph.NOT_A_EDGE);
        for(int i = 0; i < graph.getNodeNum(); ++i)
        {
            for(int j = 0; j < graph.getNodeNum(); ++j)
            {
                if(graph[i][j] != graph.NOT_A_EDGE)
                    addEdge(i, j, graph[i][j]);
            }
        }
    }
    MyGraphList<T> &operator =(const MyGraphList<T> &g)
    {
        setInvalidEdge(g.NOT_A_EDGE);
        setNodeNum(g.getNodeNum());
        this->edge = g.edge;
        return *this;
    }
    void setNodeNum(int nodeNum)
    {
        this->nodeNum = nodeNum;
        edge.resize(nodeNum);
    }
    void addEdge(int u, int v, const T &val)
    {
        if(nodeNum == 0)
            return;
        edge[u].emplace_back(Edge<T>(v, val));
    }
    void setInvalidEdge(const T& val)
    {
        _NOT_A_EDGE = val;
    }
    void appendNode()
    {
        nodeNum++;
        edge.emplace_back(QList<Edge<T>>());
    }
    int getNodeNum() const
    {
        return nodeNum;
    }
    QList<Edge<T>> &operator[](qsizetype index)
    {
        return this->edge[index];
    }
    const QList<Edge<T>> &operator[](qsizetype index) const
    {
        return this->edge[index];
    }
    const T& NOT_A_EDGE;
private:
    T _NOT_A_EDGE;

    int nodeNum;
    QList<QList<Edge<T>>> edge;
};

#endif // MYGRAPH_H
