#include "myarrowitem.h"
#include <QPainter>

MyArrowItem::MyArrowItem(int length, DIRECTIONS dir, ARROW_TYPE arrowtype)
{
    QPoint startP(0,0), endP, p1, p2;

    switch (dir)
    {
    case DIRECTIONS_UP:
        p1 = p2 = endP = QPoint(0,-length);
        if(arrowtype == ARROW_TYPE_HAVE)
            p1 += QPoint(-ARROW_HEIGHT, ARROW_WEIGHT), p2 += QPoint(ARROW_HEIGHT, ARROW_WEIGHT);
        mboundingRect = QRect(-ARROW_HEIGHT, -length, ARROW_HEIGHT << 1, length);
        break;
    case DIRECTIONS_RIGHT:
        p1 = p2 = endP = QPoint(length, 0);
        if(arrowtype == ARROW_TYPE_HAVE)
            p1 += QPoint(-ARROW_WEIGHT, -ARROW_HEIGHT), p2 += QPoint(-ARROW_WEIGHT, ARROW_HEIGHT);
        mboundingRect = QRect(0, -ARROW_HEIGHT, length, ARROW_HEIGHT << 1);
        break;
    case DIRECTIONS_DOWN:
        p1 = p2 = endP = QPoint(0, length);
        if(arrowtype == ARROW_TYPE_HAVE)
            p1 += QPoint(-ARROW_HEIGHT, -ARROW_WEIGHT), p2 += QPoint(ARROW_HEIGHT, -ARROW_WEIGHT);
        mboundingRect = QRect(-ARROW_HEIGHT, 0, ARROW_HEIGHT << 1, length);
        break;
    case DIRECTIONS_LEFT:
        p1 = p2 = endP = QPoint(-length, 0);
        if(arrowtype == ARROW_TYPE_HAVE)
            p1 += QPoint(ARROW_WEIGHT, ARROW_HEIGHT), p2 += QPoint(ARROW_WEIGHT, -ARROW_HEIGHT);
        mboundingRect = QRect(-length, -ARROW_HEIGHT, length, ARROW_HEIGHT << 1);
        break;
    default:
        break;
    }

    line1 = QLine(startP, endP);
    line2 = QLine(endP, p1);
    line3 = QLine(endP, p2);
}

//MyArrowItem::MyArrowItem(int length, ARROW_TYPE size, double angle)
//{
//    QPoint startP(0,0), endP, p1, p2;

//}

MyArrowItem::~MyArrowItem()
{

}

QRectF MyArrowItem::boundingRect() const
{
    return mboundingRect;
}

void MyArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawLine(line1);
    painter->drawLine(line2);
    painter->drawLine(line3);
}
