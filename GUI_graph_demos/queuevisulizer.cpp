#include "queuevisulizer.h"
#include "arrayitemvisulizer.h"
#include <QList>
const int QueueVisulizer::ITEM_WIDTH = ArrayItemVisulizer::getWIDTH();
const int QueueVisulizer::ITEM_HEIGHT = ArrayItemVisulizer::getHEIGHT();
const int QueueVisulizer::MAX_QUEUE_LEN = 10;
const int QueueVisulizer::SPACING = 10;
const int QueueVisulizer::MAX_SCENE_WIDTH = QueueVisulizer::SPACING * 4 + (QueueVisulizer::MAX_QUEUE_LEN + 1) * QueueVisulizer::ITEM_WIDTH;
const int QueueVisulizer::MAX_SCENE_HEIGHT = QueueVisulizer::ITEM_HEIGHT * 2 + QueueVisulizer::SPACING * 2;
const QPointF QueueVisulizer::POS_POPTEXT = QPoint(QueueVisulizer::SPACING, QueueVisulizer::SPACING);
const QPointF QueueVisulizer::POS_POPITEM = QueueVisulizer::POS_POPTEXT + QPoint(0, 25);
const QPointF QueueVisulizer::POS_HEADTEXT = QueueVisulizer::POS_POPTEXT + QPoint(QueueVisulizer::ITEM_WIDTH + QueueVisulizer::SPACING * 2, 0);
const QPointF QueueVisulizer::POS_HEADITEM = QueueVisulizer::POS_HEADTEXT + QPoint(0, 25);
const QString QueueVisulizer::POPTEXT = "被弹出的元素";
const QString QueueVisulizer::HAEDTEXT = "队首元素";
const QFont QueueVisulizer::FONT_INFO = QFont("consolas", 8);
const QColor QueueVisulizer::COLOR_INQUEUE = QColor(Qt::gray);
const QColor QueueVisulizer::COLOR_POP = QColor(Qt::green);
const QColor QueueVisulizer::COLOR_PUSH = QColor(Qt::yellow);
QueueVisulizer::QueueVisulizer(QWidget *parent) : QGraphicsView(parent)
{
    qDebug() << "POS_POPTEXT" << POS_POPTEXT << "POS_HEADTEXT" << POS_HEADTEXT;
    myScene = new QGraphicsScene(this);
    myScene->setSceneRect(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
    setScene(myScene);
    myScene->addText(POPTEXT)->setPos(POS_POPTEXT);
    myScene->addText(HAEDTEXT)->setPos(POS_HEADTEXT);
    QLineF upperLine(POS_HEADITEM, POS_HEADITEM + QPoint(MAX_QUEUE_LEN * ITEM_WIDTH, 0));
    QLineF lowerLine = upperLine.translated(0, ITEM_HEIGHT);
    myScene->addLine(upperLine);
    myScene->addLine(lowerLine);
    popItem = nullptr;

}

void QueueVisulizer::push(const QString &str)
{
    for(const auto &i : myQueue)
        i->setNowColor(COLOR_INQUEUE);
    ArrayItemVisulizer *tmp = new ArrayItemVisulizer(str);
    tmp->setNowColor(COLOR_PUSH);
    tmp->setNowFont(FONT_INFO);
    int index = myQueue.size();
    myQueue.push_back(tmp);
    if(index < MAX_QUEUE_LEN)
    {
        myScene->addItem(tmp);
        tmp->setPos(POS_HEADITEM + QPoint(index * ITEM_WIDTH, 0));
    }
    myScene->update(myScene->sceneRect());

}

void QueueVisulizer::pop()
{
    if(popItem != nullptr)
    {
        myScene->removeItem(popItem);
        delete popItem;
        popItem = nullptr;
    }
    popItem = myQueue.first();
    popItem->setPos(POS_POPITEM);
    popItem->setNowColor(COLOR_POP);
    myQueue.pop_front();
    for(int i = 0; i < std::min(MAX_QUEUE_LEN, (int)myQueue.size()); ++i)
    {
        ArrayItemVisulizer *tmp = myQueue[i];
        tmp->setPos(POS_HEADITEM + QPoint(i * ITEM_WIDTH, 0));
        tmp->setNowColor(COLOR_INQUEUE);
        myScene->addItem(tmp);
    }
    myScene->update(myScene->sceneRect());
}

void QueueVisulizer::clear()
{
    popItem = nullptr;
    for(int i = 10; i < myQueue.size(); ++i)
        delete myQueue[i];
    myQueue.clear();
    myScene->clear();

}
