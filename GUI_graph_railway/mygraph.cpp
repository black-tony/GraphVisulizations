#include <queue>
#include <algorithm>
#include <QList>
#include "mygraph.h"
struct E{
    int v, val;
    bool operator<(const E&oth) const
    {return val > oth.val;}
    E(int v = -1, int val = -1):
        v(v), val(val)
    {}
};
int dijkstra(MyGraphList<RailwayEdgeInfo> *g, int startId, int endId, QList<int> *route, QList<bool> *change)
{
    if(g->getNodeNum() == 0)
        return -2;
    using std::priority_queue;
//    struct
    priority_queue<E> q;
    q.push(E(startId, 0));
    QList<int> dis(g->getNodeNum(), 0x3F3F3F3F);
    QList<bool> vis(g->getNodeNum(), false);
    QList<int> pre(g->getNodeNum(), -1);
    dis[startId] = 0;
    while(!q.empty())
    {
        E tmp = q.top();
        q.pop();
        if(vis[tmp.v])
            continue;
        if(tmp.v == endId)
            break;
        vis[tmp.v] = true;
        for(const auto &i : (*g)[tmp.v])
        {
            if(dis[i.v] > dis[tmp.v] + 1)
            {
                dis[i.v] = dis[tmp.v] + 1;
                pre[i.v] = tmp.v;
                q.push(E(i.v, dis[tmp.v] + 1));
            }
        }
    }
    if(dis[endId] == 0x3f3f3f3f)
        return -1;
    route->clear();

    int curId = endId;
    while(~curId)
    {
        route->push_front(curId);
        curId = pre[curId];
    }


    change->clear();
    change->resize(route->size(), false);
    if(route->size() < 2)
        return dis[endId];
    QList<Edge<RailwayEdgeInfo>>::iterator it;
    for(it = (*g)[startId].begin(); it != (*g)[startId].end(); ++it)
        if(it->v == route->at(1))
            break;

    RailwayEdgeInfo preEdgeSet = it->val;
    for(int i = 1; i < change->size() - 1; ++i)
    {
        for(it = (*g)[(*route)[i]].begin(); it != (*g)[(*route)[i]].end(); ++it)
            if(it->v == route->at(i + 1))
                break;
        if(!it->val.intersects(preEdgeSet))
            (*change)[i] = true;
        preEdgeSet = it->val;
    }
    return dis[endId];

}
