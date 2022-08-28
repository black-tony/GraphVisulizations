#include "stackvisulizer.h"
#include "arrayitemvisulizer.h"
#include <QList>
const int StackVisulizer::ITEM_WIDTH = ArrayItemVisulizer::getWIDTH();
const int StackVisulizer::ITEM_HEIGHT = ArrayItemVisulizer::getHEIGHT();
const int StackVisulizer::MAX_STACK_LEN = 10;
const int StackVisulizer::SPACING = 10;
const int StackVisulizer::MAX_SCENE_WIDTH = StackVisulizer::SPACING * 4 + (StackVisulizer::MAX_STACK_LEN + 1) * StackVisulizer::ITEM_WIDTH;
const int StackVisulizer::MAX_SCENE_HEIGHT = StackVisulizer::ITEM_HEIGHT * 2 + StackVisulizer::SPACING * 2;
const QPointF StackVisulizer::POS_POPTEXT = QPoint(StackVisulizer::SPACING, StackVisulizer::SPACING);
const QPointF StackVisulizer::POS_POPITEM = StackVisulizer::POS_POPTEXT + QPoint(0, 25);
const QPointF StackVisulizer::POS_HEADTEXT = StackVisulizer::POS_POPTEXT + QPoint(StackVisulizer::ITEM_WIDTH + StackVisulizer::SPACING * 2, 0);
const QPointF StackVisulizer::POS_HEADITEM = StackVisulizer::POS_HEADTEXT + QPoint(0, 25);
const QString StackVisulizer::POPTEXT = "被弹出的元素";
const QString StackVisulizer::HAEDTEXT = "栈底元素";
const QFont StackVisulizer::FONT_INFO = QFont("consolas", 8);
const QColor StackVisulizer::COLOR_INSTACK = QColor(Qt::gray);
const QColor StackVisulizer::COLOR_POP = QColor(Qt::green);
const QColor StackVisulizer::COLOR_PUSH = QColor(Qt::yellow);

StackVisulizer::StackVisulizer(QWidget *parent) : QGraphicsView(parent)
{
    qDebug() << "POS_POPTEXT" << POS_POPTEXT << "POS_HEADTEXT" << POS_HEADTEXT;
    myScene = new QGraphicsScene(this);
    myScene->setSceneRect(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
    setScene(myScene);
    myScene->addText(POPTEXT)->setPos(POS_POPTEXT);
    myScene->addText(HAEDTEXT)->setPos(POS_HEADTEXT);
    QLineF upperLine(POS_HEADITEM, POS_HEADITEM + QPoint(MAX_STACK_LEN * ITEM_WIDTH, 0));
    QLineF lowerLine = upperLine.translated(0, ITEM_HEIGHT);
    QLineF bottomLine(POS_HEADITEM, POS_HEADITEM + QPointF(0, ITEM_HEIGHT));
    myScene->addLine(upperLine);
    myScene->addLine(lowerLine);
    myScene->addLine(bottomLine);
    popItem = nullptr;
}

void StackVisulizer::push(const QString &str)
{
    for(const auto &i : myStack)
        i->setNowColor(COLOR_INSTACK);
    ArrayItemVisulizer *tmp = new ArrayItemVisulizer(str);
    tmp->setNowColor(COLOR_PUSH);
    tmp->setNowFont(FONT_INFO);
    int index = myStack.size();
    myStack.push_back(tmp);
    if(index < MAX_STACK_LEN)
    {
        myScene->addItem(tmp);
        tmp->setPos(POS_HEADITEM + QPoint(index * ITEM_WIDTH, 0));
    }
    myScene->update(myScene->sceneRect());
}

void StackVisulizer::pop()
{
    if(popItem != nullptr)
    {
        myScene->removeItem(popItem);
        delete popItem;
        popItem = nullptr;
    }
    popItem = myStack.last();
    popItem->setPos(POS_POPITEM);
    popItem->setNowColor(COLOR_POP);
    myStack.pop_back();
    for(int i = 0; i < std::min(MAX_STACK_LEN, (int)myStack.size()); ++i)
    {
        ArrayItemVisulizer *tmp = myStack[i];
        tmp->setPos(POS_HEADITEM + QPoint(i * ITEM_WIDTH, 0));
        tmp->setNowColor(COLOR_INSTACK);
        myScene->addItem(tmp);
    }
    myScene->update(myScene->sceneRect());
}

void StackVisulizer::clear()
{
    popItem = nullptr;
    for(int i = 10; i < myStack.size(); ++i)
        delete myStack[i];
    myStack.clear();
    myScene->clear();
}
