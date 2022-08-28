#ifndef MYARROWITEM_H
#define MYARROWITEM_H

#include <QGraphicsItem>

class MyArrowItem : public QGraphicsItem
{
public:
    enum DIRECTIONS{DIRECTIONS_UP, DIRECTIONS_RIGHT, DIRECTIONS_DOWN, DIRECTIONS_LEFT};
    enum ARROW_TYPE{ARROW_TYPE_NONE, ARROW_TYPE_HAVE};
private:
    QRectF mboundingRect;
    QLineF line1,line2,line3;
    DIRECTIONS direction;
    ARROW_TYPE arrowSize;
public:


    explicit MyArrowItem (int length, DIRECTIONS dir = DIRECTIONS_RIGHT, ARROW_TYPE size = ARROW_TYPE_HAVE) ;
    explicit MyArrowItem (int length, double angle, ARROW_TYPE size = ARROW_TYPE_HAVE) ;
    ~MyArrowItem();

    // QGraphicsItem interface
public:
    const static int ARROW_HEIGHT = 3;
    const static int ARROW_WEIGHT = 6;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // MYARROWITEM_H
