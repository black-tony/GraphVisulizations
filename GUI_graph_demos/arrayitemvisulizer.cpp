#include "arrayitemvisulizer.h"
#include <QPainter>
const QPointF ArrayItemVisulizer::SELF_TOPLEFT = QPoint(0, 0);
const int ArrayItemVisulizer::WIDTH = 60;
const int ArrayItemVisulizer::HEIGHT = 30;
const int ArrayItemVisulizer::SPACING = 2;

const QString &ArrayItemVisulizer::getInfo() const
{
    return info;
}

void ArrayItemVisulizer::setInfo(const QString &newInfo)
{
    info = newInfo;
}

const QColor &ArrayItemVisulizer::getNowColor() const
{
    return nowColor;
}

void ArrayItemVisulizer::setNowColor(const QColor &newNowColor)
{
    nowColor = newNowColor;
}

const QFont &ArrayItemVisulizer::getNowFont() const
{
    return nowFont;
}

void ArrayItemVisulizer::setNowFont(const QFont &newNowFont)
{
    nowFont = newNowFont;
}


QRectF ArrayItemVisulizer::boundingRect() const
{
    return QRectF(SELF_TOPLEFT, QSizeF(WIDTH, HEIGHT));
}

void ArrayItemVisulizer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->drawRect(QRectF(SELF_TOPLEFT, QSizeF(WIDTH, HEIGHT)));

    painter->setPen(nowColor);
    painter->setBrush(nowColor);
    painter->drawRect(QRectF(SELF_TOPLEFT + QPointF(SPACING, SPACING), QSizeF(WIDTH - 2 * SPACING, HEIGHT - 2 * SPACING)));

    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->setFont(nowFont);
    painter->drawText(SELF_TOPLEFT + QPointF(SPACING * 2, SPACING + HEIGHT / 2), info);
}

int ArrayItemVisulizer::getHEIGHT()
{
    return HEIGHT;
}

int ArrayItemVisulizer::getWIDTH()
{
    return WIDTH;
}

QVariant ArrayItemVisulizer::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSceneHasChanged)
    {
        qDebug() << "detected Item scene change";
    }
    return QGraphicsItem::itemChange(change, value);
}

ArrayItemVisulizer::ArrayItemVisulizer(const QString &info) : info(info)
{

}
