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
        DFS,
        BFS,
        DIJ
    };
    enum NodeState{
        NODE_DEFAULT,
        NODE_START,
        NODE_END,
        NODE_ROUTE
    };
private:
    using NodeArray = QMap<qsizetype, NodeVisulizer*>;
    QGraphicsScene *myScene;
    States state;
    NodeArray nodes;
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
//    const static QColor EDGE_SPECIFIED_COLOR[30];
private:
    NodeVisulizer* getNodeByIndex(qsizetype index) const;
//    NodeVisulizer* getNodeByInfo(const QString& info) const;
    void clearGraph();

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

    MyGraphVisulizer(const MyGraphList<int> *graph, const QList<QString> *nodeInfo, QWidget *parent = nullptr);
    MyGraphVisulizer(QWidget *parent = nullptr);
    States getState() const;
    void setState(States newState);
    void setNodeStateByIndex(qsizetype index, NodeState state) const;
//    void setNodeStateByInfo(const QString &info) const;
    void setGraph(const MyGraphList<int> *graph, const QList<QString> *nodeInfo);
    void addNode(qsizetype u, const QString& nodeInfo);
    void addEdge(qsizetype u, qsizetype v, const QString &edgeInfo);
    void removeNode(qsizetype u);
    void removeEdge(qsizetype u, qsizetype v) const;
    void resetGraph() const;
    void rearrangeNodeId();
    MyGraphList<int> dumpGraphList();
private slots:
    void onNodeClicked(NodeVisulizer * node);
    void onNodeDestroyed(NodeVisulizer * node);
    void disableNodeMovable() const;
    void enableNodeMovable() const;


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:


};

#endif // MYGRAPHVISULIZER_H
