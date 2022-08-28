#ifndef EDGEVISULIZER_H
#define EDGEVISULIZER_H

#include <QGraphicsItem>
#include <QtWidgets>
class NodeVisulizer;
class MyGraphVisulizer;

class EdgeVisulizer : public QGraphicsItem
{
public:
    EdgeVisulizer(NodeVisulizer *source, NodeVisulizer *dest, MyGraphVisulizer *graphArea, const QString &edgeInfo, QGraphicsItem *parent = nullptr);


    NodeVisulizer *getSource() const;

    NodeVisulizer *getDest() const;

    const QString &getEdgeInfo() const;

    void setLineColor(const QColor &newLineColor);

    void setInfoFont(const QFont &newInfoFont);

    void adjust();

    void detach(NodeVisulizer *requestPoint);

private:
    NodeVisulizer *source, *dest;
    MyGraphVisulizer *graphArea;
    QString edgeInfo;
    QPointF sourcePoint, destPoint;
    QColor lineColor;
    QFont infoFont;
    const static int ARROW_HEIGHT;
    const static int ARROW_WEIGHT;
    const static int EDGE_FROM_NODE;
    const static int EDGE_SPACING;
    const static int EDGE_WIDTH;
    const static int TEXT_SPACING;
    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    enum {Type = UserType + 2};
    int type() const override;

    // QGraphicsItem interface
    void setEdgeInfo(const QString &newEdgeInfo);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // EDGEVISULIZER_H
