#include "edgevisulizer.h"
#include "nodevisulizer.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
const int EdgeVisulizer::ARROW_HEIGHT = 6;
const int EdgeVisulizer::ARROW_WEIGHT = 3;
const int EdgeVisulizer::EDGE_FROM_NODE = 10;
const int EdgeVisulizer::EDGE_SPACING = 15;
const int EdgeVisulizer::EDGE_WIDTH = 2;
const int EdgeVisulizer::TEXT_SPACING = 10;

EdgeVisulizer::EdgeVisulizer(NodeVisulizer *source, NodeVisulizer *dest, MyGraphVisulizer *graphArea, const QString &edgeInfo, QGraphicsItem *parent) : QGraphicsItem(parent),
    source(source),
    dest(dest),
    graphArea(graphArea),
    edgeInfo(edgeInfo)
{
//    setZValue(0);
    source->addEdge(dest, this);
    adjust();
}

NodeVisulizer *EdgeVisulizer::getSource() const
{
    return source;
}

NodeVisulizer *EdgeVisulizer::getDest() const
{
    return dest;
}

const QString &EdgeVisulizer::getEdgeInfo() const
{
    return edgeInfo;
}

void EdgeVisulizer::setLineColor(const QColor &newLineColor)
{
    lineColor = newLineColor;
//    update();
}

void EdgeVisulizer::setInfoFont(const QFont &newInfoFont)
{
    infoFont = newInfoFont;
//    update();
}

void EdgeVisulizer::adjust()
{
    if(source == nullptr || dest == nullptr)
        return;
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    prepareGeometryChange();
    if(length > qreal(EDGE_FROM_NODE * 2))
    {
        QPointF offset(line.dx() * EDGE_FROM_NODE / length, line.dy() * EDGE_FROM_NODE / length);
        sourcePoint = line.p1() + offset;
        destPoint = line.p2() - offset;
    }
    else
    {
        sourcePoint = destPoint = line.p1();
    }

}

void EdgeVisulizer::detach()
{
    source->removeEdge(dest);
    source = nullptr;
    dest = nullptr;
}

QRectF EdgeVisulizer::boundingRect() const
{
    if(source == nullptr || dest == nullptr)
        return QRectF();
    QPointF leftTop(std::min(sourcePoint.x(), destPoint.x()) - ARROW_HEIGHT, std::min(sourcePoint.y(), destPoint.y()) - ARROW_HEIGHT);
    QPointF rightBottom(std::max(sourcePoint.x(), destPoint.x()) + ARROW_HEIGHT, std::max(sourcePoint.y(), destPoint.y()) + ARROW_HEIGHT);
    return QRectF(leftTop, rightBottom);
//    return QRectF(sourcePoint, destPoint).normalized().adjusted(-EDGE_SPACING, -EDGE_SPACING, EDGE_SPACING, EDGE_SPACING);
}

void EdgeVisulizer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(source == nullptr || dest == nullptr)
        return;
    QLineF line(sourcePoint, destPoint);
    QLineF normalLine(line.normalVector());
    QPointF arrowHOffset(-line.unitVector().dx() * ARROW_HEIGHT,
                         -line.unitVector().dy() * ARROW_HEIGHT);
    QPointF arrowWOffset(normalLine.unitVector().dx() * ARROW_WEIGHT,
                         normalLine.unitVector().dy() * ARROW_WEIGHT);
    QPointF arrowEnd1(destPoint + arrowHOffset + arrowWOffset);
    QPointF arrowEnd2(destPoint + arrowHOffset - arrowWOffset);

    if(qFuzzyCompare(line.length(), 0.0))
        return;
    painter->setPen(QPen(lineColor, EDGE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->drawLine(line);
    painter->drawLine(destPoint, arrowEnd1);
    painter->drawLine(destPoint, arrowEnd2);

    QPointF midPos = (destPoint + sourcePoint) / 2;
    normalLine.setLength(TEXT_SPACING);
    QPointF textPos = midPos + QPoint(normalLine.dx(), normalLine.dy());
    painter->setFont(infoFont);
    painter->drawText(textPos, edgeInfo);
}

int EdgeVisulizer::type() const
{
    return Type;
}

void EdgeVisulizer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Right Button Clicked edge" << edgeInfo;
    }
    QGraphicsItem::mousePressEvent(event);
}

void EdgeVisulizer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);

}
