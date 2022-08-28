#include "mygraphvisulizer.h"
#include "nodevisulizer.h"
#include "edgevisulizer.h"
#include <QRandomGenerator>
#include <QStringList>
const int MyGraphVisulizer::SCENE_MAX_WIDTH = 800;
const int MyGraphVisulizer::SCENE_MAX_HEIGHT = 500;
const QColor MyGraphVisulizer::EDGE_DEFAULT_COLOR =  QColor(Qt::black);
const QColor MyGraphVisulizer::NODE_DEFAULT_COLOR = QColor(Qt::black);
const QColor MyGraphVisulizer::NODE_START_COLOR = QColor(Qt::green);
const QColor MyGraphVisulizer::NODE_END_COLOR = QColor(Qt::red);
const QColor MyGraphVisulizer::NODE_CHANGE_COLOR = QColor(Qt::magenta);
const QColor MyGraphVisulizer::NODE_ROUTE_COLOR = QColor(Qt::blue);
const QFont MyGraphVisulizer::EDGE_DEFAULT_FONT = QFont("consolas", 8);
const QFont MyGraphVisulizer::NODE_DEFAULT_FONT = QFont("consolas", 8);
static QColor EDGE_SPECIFIED_COLOR[30] = {
    QColor(QRgb(0xE3022A)), QColor(QRgb(0x82BF24)), QColor(QRgb(0xFBD601)), QColor(QRgb(0x461D85)),
    QColor(QRgb(0x944B9A)), QColor(QRgb(0xE20066)), QColor(QRgb(0xEC6E00)), QColor(QRgb(0x0095D9)),
    QColor(QRgb(0x87C9EC)), QColor(QRgb(0xC7AFD3)), QColor(QRgb(0x861A2A)), QColor(QRgb(0x00785F)),
    QColor(QRgb(0xE799C0)), QColor(QRgb(0x616020)), QColor(QRgb(0xCAB48F)), QColor(QRgb(0x98D1C0)),
    QColor(QRgb(0xBC7970)), QColor(QRgb(0xC4984F)), QColor(QRgb(0x48864D)), QColor(QRgb(0x666666)),
    QColor(QRgb(0xD6C66C)), QColor(QRgb(0xE0815E)), QColor(QRgb(0x6BB392)), QColor(QRgb(0xB6B6B6)),
    QColor(QRgb(0x0E7572)), QColor(QRgb(0x000000)), QColor(QRgb(0x266883)), QColor(QRgb(0x733C54)),
    QColor(QRgb(0x0033c9)), QColor(QRgb(0x0033c9))

};
MyGraphVisulizer::States MyGraphVisulizer::getState() const
{
    return state;
}

void MyGraphVisulizer::setState(States newState)
{
    state = newState;
}

void MyGraphVisulizer::setNodeStateByIndex(qsizetype index, NodeState state) const
{
    NodeArray::const_iterator it = nodes.find(index);
    if(it == nodes.end())
        return;

    switch (state)
    {
    case NODE_DEFAULT:
        it.value()->changeColor(NODE_DEFAULT_COLOR);
        break;
    case NODE_START:
        it.value()->changeColor(NODE_START_COLOR);
        break;
    case NODE_ROUTE:
        it.value()->changeColor(NODE_ROUTE_COLOR);
        break;
    case NODE_END:
        it.value()->changeColor(NODE_END_COLOR);
        break;
    default:
        break;
    }
    it.value()->update();
}

void MyGraphVisulizer::setGraph(const MyGraphList<RailwayEdgeInfo> *graph, const QList<QString> *nodeInfo)
{
    qDebug() << "INTO setgraph" ;
    clearGraph();
    setState(POINTER);
    qDebug() << "INTO 2" ;

    for(int i = 0; i < graph->getNodeNum(); ++i)
        addNode(i, nodeInfo->at(i));
    for(int i = 0; i < graph->getNodeNum(); ++i)
        for(const auto &j : (*graph)[i])
            addEdge(i, j.v, translateEdgeInfo(j.val));


}

void MyGraphVisulizer::addNode(qsizetype u, const QString &nodeInfo)
{
    QRandomGenerator rand(QRandomGenerator::global()->generate());
    QPoint nodePos(rand.bounded(25, SCENE_MAX_WIDTH - 25), rand.bounded(25, SCENE_MAX_HEIGHT - 25));
    addNode(u, nodeInfo, nodePos);
}
void MyGraphVisulizer::addNode(qsizetype u, const QString &nodeInfo, QPointF pos)
{
    if(nodes.find(u) != nodes.end())
    {
        qDebug() << "Add Node FAIL 1";
        return;
    }
    if(nodeInfos.find(nodeInfo) != nodeInfos.end())
    {
        qDebug() << "Add Node FAIL 2";
        return;
    }
    curUniqueId = std::max((qsizetype)curUniqueId, u + 1);
    nodeInfos.insert(nodeInfo);
    emit nodeStrChanged();
    qDebug() << "Add Node 1";

    NodeVisulizer *curNode = new NodeVisulizer(this);
//    QRandomGenerator rand(QRandomGenerator::global()->generate());
//    QPoint nodePos(rand.bounded(25, SCENE_MAX_WIDTH - 25), rand.bounded(25, SCENE_MAX_HEIGHT - 25));
    qDebug() << "Add Node 1";

    nodes.insert(u, curNode);
    curNode->setPos(pos);
    curNode->changeColor(NODE_DEFAULT_COLOR);
    curNode->changeFont(NODE_DEFAULT_FONT);
    curNode->setUniqueID(u);
    curNode->setInsideInfo(nodeInfo);

    connect(curNode, &NodeVisulizer::nodeClicked, this, &MyGraphVisulizer::onNodeClicked);
    connect(curNode, &NodeVisulizer::nodeDestroyed, this, &MyGraphVisulizer::onNodeDestroyed);
    myScene->addItem(curNode);
    qDebug() << "Add Node Finish!";
}
void MyGraphVisulizer::addEdge(qsizetype u, qsizetype v, const QString &edgeInfo)
{
    NodeVisulizer *source, *dest;
    NodeArray::const_iterator it = nodes.constFind(u);
    if(it == nodes.cend())
        return;
    source = it.value();
    it = nodes.constFind(v);
    if(it == nodes.cend())
        return;
    dest = it.value();
    if(source == dest)
        return;
    addEdge(source, dest, edgeInfo);


}
void MyGraphVisulizer::addEdge(NodeVisulizer *source, NodeVisulizer *dest, const QString &edgeInfo)
{
    QString nowEdgeInfo = edgeInfo;
    EdgeVisulizer *nowEdge = nullptr;
    for(NodeVisulizer::NodeEdgeArray::iterator it = source->getEdges().begin(); it != source->getEdges().end(); ++it)
    {
        if(it->first == dest)
        {
            nowEdge = it->second;
            break;
        }
    }
    RailwayEdgeInfo nowEdgeSet = getEdgeInfoSet(edgeInfo);

    if(nowEdge == nullptr)
        nowEdge = new EdgeVisulizer(source, dest, this, nowEdgeInfo);
    else
    {
        RailwayEdgeInfo orignset = getEdgeInfoSet(nowEdge->getEdgeInfo());
        nowEdgeSet.unite(orignset);
        nowEdgeInfo = translateEdgeInfo(nowEdgeSet);
        nowEdge->setEdgeInfo(nowEdgeInfo);
    }
    if(nowEdgeSet.size() > 1)
        nowEdge->setLineColor(EDGE_DEFAULT_COLOR);
    else
        nowEdge->setLineColor((*nowEdgeSet.begin()) < 30 ? EDGE_SPECIFIED_COLOR[(*nowEdgeSet.begin())] : EDGE_DEFAULT_COLOR);
    myScene->addItem(nowEdge);


}
RailwayEdgeInfo MyGraphVisulizer::getEdgeInfoSet(const QString& tmpstr) const
{
    RailwayEdgeInfo tmp;
    QStringList strlist = tmpstr.split(' ', Qt::SkipEmptyParts);
    for(const auto &i : strlist)
        tmp.insert(i.toInt());
    return tmp;
}
QString MyGraphVisulizer::translateEdgeInfo(const RailwayEdgeInfo& tmpset) const
{
    QString tmp = "";
    bool isFirst = true;
    for(const auto &k : tmpset)
    {
        if(!isFirst)
            tmp += " ";
        else
            isFirst = false;
        tmp+= QString::number(k);
    }
    return tmp;
}
void MyGraphVisulizer::removeNode(qsizetype u)
{
    Q_UNUSED(u);
    return;
}

void MyGraphVisulizer::removeEdge(qsizetype u, qsizetype v) const
{
    NodeVisulizer *source, *dest;
    NodeArray::const_iterator it = nodes.constFind(u);
    if(it == nodes.cend())
        return;
    source = it.value();
    it = nodes.constFind(v);
    if(it == nodes.cend())
        return;
    dest = it.value();
    removeEdge(source, dest);

}
void MyGraphVisulizer::removeEdge(NodeVisulizer * source, NodeVisulizer * dest) const
{
    if(source == dest)
        return;
    EdgeVisulizer *nowEdge = source->isConnect(dest);
    if(nowEdge == nullptr)
        return;
    nowEdge->detach(source);
    nowEdge->detach(dest);
    myScene->removeItem(nowEdge);
    delete nowEdge;
}
void MyGraphVisulizer::resetGraph() const
{
    for(auto &i : nodes)
    {
        i->enableMove();
        i->changeColor(NODE_DEFAULT_COLOR);
        i->update();
//        for(auto &j : i->getEdges())
//        {
//            j.second->setLineColor(EDGE_DEFAULT_COLOR);
//            j.second->update();
//        }
    }

}

void MyGraphVisulizer::rearrangeNodeId()
{
    NodeArray tmp;
    int curId = 0;
    for(NodeArray::const_iterator it = nodes.cbegin(); it != nodes.cend(); ++it, ++curId)
    {
        it.value()->setUniqueID(curId);
        tmp.insert(curId, it.value());
    }
    curUniqueId = curId;
    nodes.swap(tmp);
}
void MyGraphVisulizer::cancelStartNode()
{
    if(startNode != nullptr)
    {
        startNode->changeColor(NODE_DEFAULT_COLOR);
        startNode->update();
        startNode = nullptr;
    }

}

void MyGraphVisulizer::highlightRoute(QList<int> *route, QList<bool> *change) const
{
    if(route->size() < 2)
        return;
    qDebug() << "inside";
    NodeVisulizer *tmp;
    tmp = getNodeByIndex(route->at(0));
    tmp->changeColor(NODE_START_COLOR);
    tmp->update();
    tmp = getNodeByIndex(route->at(route->size() - 1));
    tmp->changeColor(NODE_END_COLOR);
    tmp->update();
    for(int i = 1; i < route->size() - 1; ++i)
    {
        tmp = getNodeByIndex(route->at(i));
        tmp->changeColor((*change)[i] ? NODE_CHANGE_COLOR : NODE_ROUTE_COLOR);
        tmp->update();
    }
//    myScene->update(myScene->sceneRect());
}
void MyGraphVisulizer::onNodeClicked(NodeVisulizer *node)
{
    if(node == startNode)
        return;
    if(state == ADD_EDGE && startNode != nullptr)
    {
        int railwayId = inputEdgeInfo(node, startNode);
        addEdge(startNode, node, QString::number(railwayId));
        cancelStartNode();
    }
    else if(state == DEL_EDGE && startNode != nullptr)
    {
        removeEdge(startNode, node);
        cancelStartNode();
    }
    else if((state == ADD_EDGE || state == DEL_EDGE) && startNode == nullptr)
    {
        startNode = node;
        startNode->changeColor(NODE_START_COLOR);
        startNode->update();
    }
    return;
}

void MyGraphVisulizer::onNodeDestroyed(NodeVisulizer *node, const QString& nodeInfo)
{
//    Q_UNUSED(node);
    qDebug() << "ACCEPT NODE DESTORYED";
    NodeArray::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it)
        if(it.value() == node)
            break;
    nodes.erase(it);
    nodeInfos.remove(nodeInfo);
    emit nodeStrChanged();

    return;
}

void MyGraphVisulizer::disableNodeMovable() const
{
    for(auto &i : nodes)
        i->disableMove();
}

void MyGraphVisulizer::enableNodeMovable() const
{
    for(auto &i : nodes)
        i->enableMove();
}

MyGraphList<RailwayEdgeInfo> MyGraphVisulizer::dumpGraphList(QList<QString> *nodeInfosstr)
{
    //在dump的时候顺便整理id
    rearrangeNodeId();
    nodeInfosstr->clear();//这里需要更改
    nodeInfosstr->resize(nodes.size(), "");
    MyGraphList<RailwayEdgeInfo> tmpgraph;
    tmpgraph.setNodeNum(nodes.size());
    for(NodeArray::const_iterator i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        (*nodeInfosstr)[i.key()] = i.value()->getInsideInfo();
        for(const auto &j : i.value()->getEdges())
        {

            NodeArray::const_iterator it = nodes.cbegin();
            for(; it != nodes.cend(); ++it)
                if(it.value() == j.first)
                    break;
            if(it == nodes.cend())
                qDebug() << "ERROR : InValid Node!";
            else
                tmpgraph.addEdge(i.key(), it.key(), getEdgeInfoSet(j.second->getEdgeInfo()));
        }
    }
    return tmpgraph;
}
void MyGraphVisulizer::showErrorMessage(const QString &message)
{
    QMessageBox infoBox(this);
    infoBox.setIcon(QMessageBox::Icon::Critical);
    infoBox.setText(message);
    infoBox.setWindowTitle("错误提示");
    infoBox.exec();
}
QString MyGraphVisulizer::inputNodeInfo()
{
    bool ok = false;
    QString newStationName = "";
    while(!ok || newStationName.isEmpty())
    {
        newStationName = QInputDialog::getText(this,
                                               "输入新增站点名",
                                               "请输入要新增的站点名, 不能是空串或与之前重复",
                                               QLineEdit::Normal,
                                               "", &ok);
        if(!ok)
            showErrorMessage("请输入新增站点名!");
        else if(newStationName.isEmpty())
            showErrorMessage("输入的站点名是空串!");
        else if(nodeInfos.find(newStationName) != nodeInfos.end())
        {
            showErrorMessage("输入的站点名重复!");
            newStationName = "";
        }
    }
    return newStationName;
}
int MyGraphVisulizer::inputEdgeInfo(NodeVisulizer *u, NodeVisulizer *v)
{
    Q_UNUSED(u);
    Q_UNUSED(v);
    bool ok = false;
    int newRailWayID = -1;
    while(!ok || (newRailWayID < 0) || (newRailWayID > 99))
    {
        newRailWayID = QInputDialog::getInt(this,
                                               "输入新增边的地铁线路编号",
                                               "请输入要新增的地铁线路编号",
                                              -1, 0, 99, 1,
                                               &ok);
        if(!ok)
            showErrorMessage("输入新增边的地铁线路编号!");
        else if((newRailWayID < 0) || (newRailWayID > 99))
            showErrorMessage("输入的地铁线路编号不符合要求!");
    }
    return newRailWayID;
}
void MyGraphVisulizer::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->buttons() != Qt::LeftButton)
        return;
    switch (state)
    {
    case ADD_NODE:
        cancelStartNode();
        addNode(curUniqueId++, inputNodeInfo(), event->pos());
        break;
    default:
        break;
    }
}

NodeVisulizer *MyGraphVisulizer::getNodeByIndex(qsizetype index) const
{
    return nodes[index];
}

void MyGraphVisulizer::clearGraph()
{

    qDebug() << "INTO CLEAR GRAPH";
    if(myScene == NULL)
    {
        myScene = new QGraphicsScene(this);
        myScene->setSceneRect(0, 0, SCENE_MAX_WIDTH, SCENE_MAX_HEIGHT);
        this->resize(SCENE_MAX_WIDTH + 10, SCENE_MAX_HEIGHT + 10);
        setScene(myScene);

    }
    myScene->dumpObjectTree();
    myScene->clear();

    setState(POINTER);
    nodes.clear();

}

MyGraphVisulizer::MyGraphVisulizer(const MyGraphList<RailwayEdgeInfo> *graph, const QList<QString> *nodeInfo, QWidget *parent) : QGraphicsView(parent)
{
    startNode = nullptr;
    curUniqueId = 0;
    qDebug() << "inside!";
    myScene = NULL;
    setGraph(graph, nodeInfo);
}

MyGraphVisulizer::MyGraphVisulizer(QWidget *parent) : QGraphicsView(parent)
{
    curUniqueId = 0;
    startNode = nullptr;
    myScene = NULL;
    clearGraph();
    setState(POINTER);
    nodeInfos.clear();
    nodes.clear();

}
