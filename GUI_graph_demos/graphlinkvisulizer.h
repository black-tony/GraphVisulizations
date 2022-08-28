#ifndef GRAPHLINKVISULIZER_H
#define GRAPHLINKVISULIZER_H

#include <QGraphicsView>
#include <QWidget>

#include "mygraph.h"
class GraphLinkVisulizer : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene *scene;
    void addLinkList(int offsetX, int offsetY, const QList<int> *array);
    QPoint calcNextPos(QPoint pre, bool isNewLine = false);
    void addNode(const QString &info, QPoint coord, bool isEnd = false, bool isFirst = false);
    void addArrow(QPoint curpos, bool isEnd = false);
    const static int SCENE_MAX_WIDTH;
    const static int SCENE_MAX_HEIGHT;
    const static int NODE_RECT_VAL_W;
    const static int NODE_RECT_POINTER_W;
    const static int NODE_RECT_H;
    const static int NODE_SPACING_V; //一个node对应的space_v在node上方
    const static int NODE_SPACING_H; //一个node对应的space_h在node右方
    const static int ARROW_LEN;
    const static int ARROW_H_OFFSET;
    const static int ARROW_V_OFFSET;
    const static int NODE_WIDTH;
    const static int NODE_HEIGHT;
    const static QBrush HEAD_FILL;
    const static QBrush NODE_FILL;


public:
//    GraphLinkVisulizer(QWidget *parent = nullptr);
    GraphLinkVisulizer(QWidget *parent = nullptr);
    GraphLinkVisulizer(const MyGraphList<int> *graph, QWidget *parent = nullptr);
    void setMyGraph(const MyGraphList<int> *graph);

};

#endif // GRAPHLINKVISULIZER_H
