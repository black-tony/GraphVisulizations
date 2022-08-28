#include "graphlinkvisulizer.h"
#include <QGraphicsItem>
#include "myarrowitem.h"
const int GraphLinkVisulizer::SCENE_MAX_WIDTH = 750;
const int GraphLinkVisulizer::SCENE_MAX_HEIGHT = 590;
const int GraphLinkVisulizer::NODE_RECT_VAL_W = 60;
const int GraphLinkVisulizer::NODE_RECT_POINTER_W = 25;
const int GraphLinkVisulizer::NODE_RECT_H = 30;
const int GraphLinkVisulizer::NODE_SPACING_V = 10; //一个node对应的space_v在node上方
const int GraphLinkVisulizer::NODE_SPACING_H = 20; //一个node对应的space_h在node右方
const int GraphLinkVisulizer::ARROW_LEN = (NODE_SPACING_H + NODE_RECT_POINTER_W / 2);
const int GraphLinkVisulizer::ARROW_H_OFFSET = (NODE_RECT_VAL_W + NODE_RECT_POINTER_W / 2);
const int GraphLinkVisulizer::ARROW_V_OFFSET = (NODE_SPACING_V + NODE_RECT_H / 2);
const int GraphLinkVisulizer::NODE_WIDTH = (NODE_RECT_POINTER_W + NODE_RECT_VAL_W + NODE_SPACING_H);
const int GraphLinkVisulizer::NODE_HEIGHT = (NODE_SPACING_V * 2 + NODE_RECT_H);
const QBrush GraphLinkVisulizer::HEAD_FILL = QBrush(Qt::yellow);
const QBrush GraphLinkVisulizer::NODE_FILL = QBrush(Qt::green);
void GraphLinkVisulizer::addLinkList(int offsetX, int offsetY, const QList<int> *array)
{
    Q_UNUSED(offsetX);
    Q_UNUSED(offsetY);
    Q_UNUSED(array);
    return;
}

QPoint GraphLinkVisulizer::calcNextPos(QPoint pre, bool isNewLine)
{
    QPoint nextpos;
    if(isNewLine)
    {
        nextpos.setX(NODE_SPACING_H);
        nextpos.setY(pre.y() + NODE_HEIGHT);
    }
    else
    {
        nextpos.setX((pre.x() + NODE_WIDTH * 2 > SCENE_MAX_WIDTH) ? NODE_SPACING_H : (pre.x() + NODE_WIDTH));
        nextpos.setY((pre.x() + NODE_WIDTH * 2 > SCENE_MAX_WIDTH) ? pre.y() + NODE_WIDTH : pre.y());
    }
    return nextpos;
}

void GraphLinkVisulizer::addNode(const QString &info, QPoint coord, bool isEnd, bool isFirst)
{
    int x = coord.x(), y = coord.y();
    scene->addRect(x, y + NODE_SPACING_V, NODE_RECT_VAL_W, NODE_RECT_H, QPen(), isFirst ? HEAD_FILL : NODE_FILL);
    scene->addRect(x + NODE_RECT_VAL_W, y + NODE_SPACING_V, NODE_RECT_POINTER_W, NODE_RECT_H);
    scene->addText(info, QFont("Consolas", 8))->setPos(x, y + NODE_SPACING_V + 5);
    if(isEnd)
        scene->addText(" ^", QFont("Consolas", 8))->setPos(x + NODE_RECT_VAL_W, y + NODE_SPACING_V + 5);

}

void GraphLinkVisulizer::addArrow(QPoint curpos, bool isEnd)
{
    if(isEnd)
        return;
    QPoint nextpos = calcNextPos(curpos);
    MyArrowItem *pArrow;
    if(nextpos.y() == curpos.y())
    {
        pArrow = new MyArrowItem(ARROW_LEN);
        pArrow->setPos(curpos.x() + ARROW_H_OFFSET, curpos.y() + ARROW_V_OFFSET);
        scene->addItem(pArrow);

    }
    else
    {
        QPoint startPoint1(curpos.x() + ARROW_H_OFFSET, curpos.y() + ARROW_V_OFFSET);
        QPoint startPoint2(startPoint1.x(), startPoint1.y() + NODE_HEIGHT / 2);
        QPoint startPoint3(NODE_SPACING_H + NODE_RECT_POINTER_W / 2, startPoint2.y());

        pArrow = new MyArrowItem(ARROW_LEN, MyArrowItem::DIRECTIONS_DOWN, MyArrowItem::ARROW_TYPE_NONE);
        pArrow->setPos(startPoint1);
        scene->addItem(pArrow);
        pArrow = new MyArrowItem(startPoint2.x() - startPoint3.x(), MyArrowItem::DIRECTIONS_LEFT, MyArrowItem::ARROW_TYPE_NONE);
        pArrow->setPos(startPoint2);
        scene->addItem(pArrow);
        pArrow = new MyArrowItem(NODE_SPACING_V, MyArrowItem::DIRECTIONS_DOWN);
        pArrow->setPos(startPoint3);
        scene->addItem(pArrow);
    }
}

void GraphLinkVisulizer::setMyGraph(const MyGraphList<int> *graph)
{
    if(scene != NULL)
    {
        scene->clear();
        delete scene;
    }
    QPoint curpos(NODE_SPACING_H, 0);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, SCENE_MAX_WIDTH, SCENE_MAX_HEIGHT);
    this->setScene(scene);
    for(int i = 0; i < graph->getNodeNum(); ++i)
    {
        //先添加链表头
        const QList<Edge<int>> & nowlist = (*graph)[i];

        addNode(QString::number(i), curpos, nowlist.empty(), true);
        addArrow(curpos, nowlist.empty());
        curpos = calcNextPos(curpos, nowlist.empty());

        for(QList<Edge<int>>::const_iterator it = nowlist.cbegin(); it != nowlist.cend(); ++it)
        {
            addNode(QString("(") + QString::number(it->val) + "," + QString::number(it->v) + QString(")"), curpos,
                    (it + 1) == nowlist.cend());
            addArrow(curpos, (it + 1) == nowlist.cend());
            curpos = calcNextPos(curpos, (it + 1) == nowlist.cend());
        }

    }

}

GraphLinkVisulizer::GraphLinkVisulizer(QWidget *parent) : QGraphicsView(parent)
{
    scene = NULL;
}
GraphLinkVisulizer::GraphLinkVisulizer(const MyGraphList<int> *graph, QWidget *parent) : QGraphicsView(parent)
{
    scene = NULL;
    setMyGraph(graph);
}
