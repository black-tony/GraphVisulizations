#ifndef STACKVISULIZER_H
#define STACKVISULIZER_H

#include <QGraphicsView>
#include <QFont>
#include <QColor>
class ArrayItemVisulizer;
class StackVisulizer : public QGraphicsView
{
    Q_OBJECT
private:
    using VectorItemArray = QList<ArrayItemVisulizer*>;
    VectorItemArray myStack;
    ArrayItemVisulizer* popItem;
    QGraphicsScene *myScene;
    const static int MAX_STACK_LEN;
    const static int MAX_SCENE_WIDTH;
    const static int MAX_SCENE_HEIGHT;
    const static int ITEM_WIDTH;
    const static int ITEM_HEIGHT;
    const static int SPACING;
    const static QPointF POS_POPTEXT;
    const static QPointF POS_POPITEM;
    const static QPointF POS_HEADTEXT;
    const static QPointF POS_HEADITEM;
    const static QString POPTEXT;
    const static QString HAEDTEXT;
    const static QFont FONT_INFO;
    const static QColor COLOR_INSTACK;
    const static QColor COLOR_POP;
    const static QColor COLOR_PUSH;
public:
    StackVisulizer(QWidget *parent = nullptr);
    void push(const QString& str);
    void pop();
    void clear();
};

#endif // STACKVISULIZER_H
