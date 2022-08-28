#ifndef QUEUEVISULIZER_H
#define QUEUEVISULIZER_H

#include <QGraphicsView>
#include <QFont>
#include <QColor>
class ArrayItemVisulizer;
class QueueVisulizer : public QGraphicsView
{
    Q_OBJECT
private:
    using VectorItemArray = QList<ArrayItemVisulizer*>;
    VectorItemArray myQueue;
    ArrayItemVisulizer* popItem;
    QGraphicsScene *myScene;
    const static int MAX_QUEUE_LEN;
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
    const static QColor COLOR_INQUEUE;
    const static QColor COLOR_POP;
    const static QColor COLOR_PUSH;

public:
    QueueVisulizer(QWidget *parent = nullptr);
    void push(const QString& str);
    void pop();
    void clear();
};

#endif // QUEUEVISULIZER_H
