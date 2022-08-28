#include "mygraphvisulizer.h"
#include "nodevisulizer.h"
#include "edgevisulizer.h"
#include <QRandomGenerator>
const int MyGraphVisulizer::SCENE_MAX_WIDTH = 800;
const int MyGraphVisulizer::SCENE_MAX_HEIGHT = 600;
const QColor MyGraphVisulizer::EDGE_DEFAULT_COLOR =  QColor(Qt::black);
const QColor MyGraphVisulizer::NODE_DEFAULT_COLOR = QColor(Qt::black);
const QColor MyGraphVisulizer::NODE_START_COLOR = QColor(Qt::green);
const QColor MyGraphVisulizer::NODE_END_COLOR = QColor(Qt::red);
const QColor MyGraphVisulizer::NODE_ROUTE_COLOR = QColor(Qt::blue);
const QFont MyGraphVisulizer::EDGE_DEFAULT_FONT = QFont("consolas", 8);
const QFont MyGraphVisulizer::NODE_DEFAULT_FONT = QFont("consolas", 8);

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

void MyGraphVisulizer::setGraph(const MyGraphList<int> *graph, const QList<QString> *nodeInfo)
{
    qDebug() << "INTO setgraph" ;
    clearGraph();
    setState(POINTER);
    qDebug() << "INTO 2" ;

    for(int i = 0; i < graph->getNodeNum(); ++i)
        addNode(i, nodeInfo->at(i));
    for(int i = 0; i < graph->getNodeNum(); ++i)
        for(const auto &j : (*graph)[i])
            addEdge(i, j.v, QString::number(j.val));


}

void MyGraphVisulizer::addNode(qsizetype u, const QString &nodeInfo)
{
    if(nodes.find(u) != nodes.end())
        return;
    qDebug() << "Add Node 1";

    NodeVisulizer *curNode = new NodeVisulizer(this);
    QRandomGenerator rand(QRandomGenerator::global()->generate());
    QPoint nodePos(rand.bounded(25, SCENE_MAX_WIDTH - 25), rand.bounded(25, SCENE_MAX_HEIGHT - 25));
    qDebug() << "Add Node 1";

    nodes.insert(u, curNode);
    curNode->setPos(nodePos);
    curNode->changeColor(NODE_DEFAULT_COLOR);
    curNode->changeFont(NODE_DEFAULT_FONT);
    curNode->setUniqueID(u);
    curNode->setInsideInfo(nodeInfo);

    connect(curNode, &NodeVisulizer::nodeClicked, this, &MyGraphVisulizer::onNodeClicked);
    connect(curNode, &NodeVisulizer::destoryNode, this, &MyGraphVisulizer::onNodeDestroyed);
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
    EdgeVisulizer *nowEdge = new EdgeVisulizer(source, dest, this, edgeInfo);
    myScene->addItem(nowEdge);


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
    if(source == dest)
        return;
    EdgeVisulizer *nowEdge = source->removeEdge(dest);
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
        for(auto &j : i->getEdges())
        {
            j.second->setLineColor(EDGE_DEFAULT_COLOR);
            j.second->update();
        }
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
    nodes.swap(tmp);
}

void MyGraphVisulizer::onNodeClicked(NodeVisulizer *node)
{
    Q_UNUSED(node);
    return;
}

void MyGraphVisulizer::onNodeDestroyed(NodeVisulizer *node)
{
    Q_UNUSED(node);

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

MyGraphList<int> MyGraphVisulizer::dumpGraphList()
{
    //在dump的时候顺便整理id
    rearrangeNodeId();
    MyGraphList<int> tmpgraph;
    tmpgraph.setNodeNum(nodes.size());
    tmpgraph.setInvalidEdge(0);//这里用不到
    for(NodeArray::const_iterator i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        for(const auto &j : i.value()->getEdges())
        {

            NodeArray::const_iterator it = nodes.cbegin();
            for(; it != nodes.cend(); ++it)
                if(it.value() == j.first)
                    break;
            if(it == nodes.cend())
                qDebug() << "ERROR : InValid Node!";
            else
                tmpgraph.addEdge(i.key(), it.key(), j.second->getEdgeInfo().toInt());
        }
    }
    return tmpgraph;
}

void MyGraphVisulizer::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->buttons() != Qt::LeftButton)
        return;
    switch (state)
    {
    case POINTER:
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
        setScene(myScene);

    }
    myScene->dumpObjectTree();
    myScene->clear();

    setState(POINTER);
    nodes.clear();

}

MyGraphVisulizer::MyGraphVisulizer(const MyGraphList<int> *graph, const QList<QString> *nodeInfo, QWidget *parent) : QGraphicsView(parent)
{
    qDebug() << "inside!";
    myScene = NULL;

    setGraph(graph, nodeInfo);
}

MyGraphVisulizer::MyGraphVisulizer(QWidget *parent) : QGraphicsView(parent)
{
    myScene = NULL;

    myScene = new QGraphicsScene(this);

    setScene(myScene);
    setSceneRect(0, 0, SCENE_MAX_WIDTH, SCENE_MAX_HEIGHT);

}
