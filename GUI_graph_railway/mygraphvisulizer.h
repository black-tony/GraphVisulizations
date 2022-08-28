#ifndef MYGRAPHVISULIZER_H
#define MYGRAPHVISULIZER_H

#include <QGraphicsView>
#include <QWidget>
#include <QList>
#include <QMap>
#include <QtWidgets>

#include "mygraph.h"
class NodeVisulizer;
class MyGraphVisulizer : public QGraphicsView
{
    Q_OBJECT
public:
    enum States{
        POINTER,
        ADD_EDGE,
        ADD_NODE,
        RUNNING_ALG,
        DEL_EDGE,
        DEL_NODE
    };
    enum NodeState{
        NODE_DEFAULT,
        NODE_START,
        NODE_END,
        NODE_ROUTE
    };
    QSet<QString> nodeInfos;
private:
    using NodeArray = QMap<qsizetype, NodeVisulizer*>;
    QGraphicsScene *myScene;
    States state;
    NodeArray nodes;
    NodeVisulizer *startNode;
    int curUniqueId;
//    QMap<QString, qsizetype> infoArray;

    const static int SCENE_MAX_WIDTH;
    const static int SCENE_MAX_HEIGHT;
    const static QColor EDGE_DEFAULT_COLOR;
    const static QColor NODE_DEFAULT_COLOR;
    const static QColor NODE_START_COLOR;
    const static QColor NODE_END_COLOR;
    const static QColor NODE_ROUTE_COLOR;
    const static QFont EDGE_DEFAULT_FONT;
    const static QFont NODE_DEFAULT_FONT;
    const static QColor NODE_CHANGE_COLOR;
private:
    NodeVisulizer* getNodeByIndex(qsizetype index) const;
//    NodeVisulizer* getNodeByInfo(const QString& info) const;
    void clearGraph();

    void addNode(qsizetype u, const QString &nodeInfo, QPointF pos);
    QString inputNodeInfo();
    void showErrorMessage(const QString &message);
    int inputEdgeInfo(NodeVisulizer *u, NodeVisulizer *v);
    RailwayEdgeInfo getEdgeInfoSet(const QString &tmp) const;
    QString translateEdgeInfo(const RailwayEdgeInfo &tmpset) const;
    void addEdge(NodeVisulizer *source, NodeVisulizer *dest, const QString &edgeInfo);
public:
//    enum States{
//        POINTER,
//        ADD_EDGE,
//        ADD_NODE,
//        DFS,
//        BFS,
//        DIJ
//    };
//    enum NodeState{
//        NODE_DEFAULT,
//        NODE_START,
//        NODE_END,
//        NODE_ROUTE
//    };

    MyGraphVisulizer(const MyGraphList<RailwayEdgeInfo> *graph, const QList<QString> *nodeInfo, QWidget *parent = nullptr);
    MyGraphVisulizer(QWidget *parent = nullptr);
    States getState() const;
    void setState(States newState);
    void setNodeStateByIndex(qsizetype index, NodeState state) const;
//    void setNodeStateByInfo(const QString &info) const;
    void setGraph(const MyGraphList<RailwayEdgeInfo> *graph, const QList<QString> *nodeInfo);
    void addNode(qsizetype u, const QString& nodeInfo);
    void addEdge(qsizetype u, qsizetype v, const QString &edgeInfo);
    void removeNode(qsizetype u);
    void removeEdge(qsizetype u, qsizetype v) const;
    void resetGraph() const;
    void rearrangeNodeId();
//    MyGraphList<RailwayEdgeInfo> dumpGraphList();
    MyGraphList<RailwayEdgeInfo> dumpGraphList(QList<QString> *nodeInfos);
    void cancelStartNode();
    void highlightRoute(QList<int> *route, QList<bool> *change) const;
    void removeEdge(NodeVisulizer *source, NodeVisulizer *dest) const;
private slots:
    void onNodeClicked(NodeVisulizer * node);
//    void onNodeDestroyed(NodeVisulizer * node);
    void disableNodeMovable() const;
    void enableNodeMovable() const;


    // QWidget interface
    void onNodeDestroyed(NodeVisulizer *node, const QString &nodeInfo);
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void nodeStrChanged();

};

#endif // MYGRAPHVISULIZER_H
