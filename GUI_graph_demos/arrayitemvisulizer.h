#ifndef ARRAYITEMVISULIZER_H
#define ARRAYITEMVISULIZER_H

#include <QGraphicsItem>
#include <QList>
#include <QFont>
#include <QColor>
class ArrayItemVisulizer : public QGraphicsItem
{
private:
    QString info;
    QColor nowColor;
    QFont nowFont;
    const static QPointF SELF_TOPLEFT;
    const static int WIDTH;
    const static int HEIGHT;
    const static int SPACING;
public:

    ArrayItemVisulizer(const QString &info = "");

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    const QString &getInfo() const;
    void setInfo(const QString &newInfo);
    const QColor &getNowColor() const;
    void setNowColor(const QColor &newNowColor);
    const QFont &getNowFont() const;
    void setNowFont(const QFont &newNowFont);
    static int getHEIGHT();
    static int getWIDTH();

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // ARRAYITEMVISULIZER_H
