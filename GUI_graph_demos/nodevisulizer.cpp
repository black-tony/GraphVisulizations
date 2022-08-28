#include "nodevisulizer.h"
#include "mygraphvisulizer.h"
#include "edgevisulizer.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
const int NodeVisulizer::NODE_DIAM = 36;
const QPoint NodeVisulizer::NODE_TOPLEFT_CORNER = QPoint(-NodeVisulizer::NODE_DIAM / 2, -NodeVisulizer::NODE_DIAM / 2);
const QPoint NodeVisulizer::TEXT_TOPLEFT_CORNER = QPoint(-5, 5);
const int NodeVisulizer::NODE_RING_DIAM = 4;

void NodeVisulizer::addEdge(NodeVisulizer *v, EdgeVisulizer *edge)
{
    outEdgeList.push_back(std::make_pair(v, edge));
    v->inEdgeList.push_back(std::make_pair(this, edge));
    edge->adjust();
}

EdgeVisulizer * NodeVisulizer::removeEdge(NodeVisulizer *v)
{
    NodeEdgeArray::const_iterator outIt;
    for(outIt = outEdgeList.cbegin(); outIt != outEdgeList.cend(); ++outIt)
    {
        if(outIt->first == v)
            break;
    }
    NodeEdgeArray::const_iterator inIt;
    for(inIt = v->inEdgeList.cbegin(); inIt != v->inEdgeList.cend(); ++inIt)
    {
        if(inIt->first == this)
            break;
    }
    if(outIt == outEdgeList.cend() || inIt == v->inEdgeList.cend())
        return nullptr;
    assert(outIt->second == inIt->second);
    outEdgeList.erase(outIt);
    v->inEdgeList.erase(inIt);
    return outIt->second;
}

const NodeVisulizer::NodeEdgeArray &NodeVisulizer::getEdges() const
{
    return outEdgeList;
}

NodeVisulizer::NodeEdgeArray &NodeVisulizer::getEdges()
{
    return outEdgeList;
}

void NodeVisulizer::changeColor(QColor color)
{
    nowColor = color;
//    update();
}

void NodeVisulizer::changeFont(QFont font)
{
    nowFont = font;
//    update();
}

void NodeVisulizer::setInsideInfo(const QString &str)
{
    informationInside = str;
//    update();
}

void NodeVisulizer::setOutsideInfo(const QString &str)
{
    informationOutside = str;
//    update();
}

int NodeVisulizer::getID() const
{
    return uniqueID;
}

void NodeVisulizer::enableMove()
{
    setFlag(ItemIsMovable, true);
}

void NodeVisulizer::disableMove()
{
    setFlag(ItemIsMovable, false);
}

QPainterPath NodeVisulizer::shape() const
{
    QPainterPath path;
    path.addEllipse(NODE_TOPLEFT_CORNER.x(), NODE_TOPLEFT_CORNER.y(), NODE_DIAM, NODE_DIAM);
    return path;
}

QRectF NodeVisulizer::boundingRect() const
{
    qreal spacing = 2;
    return QRectF(NODE_TOPLEFT_CORNER - QPoint(spacing * 2, spacing * 2), QSize(NODE_DIAM + spacing * 3, NODE_DIAM + spacing * 3));
}

void NodeVisulizer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(nowColor);
    painter->setBrush(nowColor);
    painter->drawEllipse(NODE_TOPLEFT_CORNER.x(), NODE_TOPLEFT_CORNER.y(), NODE_DIAM, NODE_DIAM);
    painter->setBrush(Qt::white);
    painter->drawEllipse(NODE_TOPLEFT_CORNER.x() + NODE_RING_DIAM / 2, NODE_TOPLEFT_CORNER.y() + NODE_RING_DIAM / 2,
                         NODE_DIAM - NODE_RING_DIAM, NODE_DIAM - NODE_RING_DIAM);
    painter->setFont(nowFont);
    painter->drawText(TEXT_TOPLEFT_CORNER, informationInside);
}

int NodeVisulizer::type() const
{
    return Type;
}

void NodeVisulizer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setFlag(ItemIsMovable, true);
        if(graphArea->getState() == MyGraphVisulizer::States::ADD_EDGE)
        {
            setFlag(ItemIsMovable, false);
            setFlag(ItemIsSelectable, true);
        }
        emit nodeClicked(this);
    }
    else if(event->button() == Qt::RightButton)
    {
        qDebug() << "Right Button Clicked" << informationInside;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void NodeVisulizer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    qDebug() << "mouse Released in" << informationInside;
}

QVariant NodeVisulizer::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        qDebug() << "position change" << informationInside <<  value.toPointF();
        for(const auto &i : outEdgeList)
            i.second->adjust();
        for(const auto &i : inEdgeList)
            i.second->adjust();
        break;
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);

}

int NodeVisulizer::getUniqueID() const
{
    return uniqueID;
}

void NodeVisulizer::setUniqueID(int newUniqueID)
{
    uniqueID = newUniqueID;
}

NodeVisulizer::NodeVisulizer(MyGraphVisulizer *graphArea, QGraphicsItem *parent) : QGraphicsObject(parent),
    graphArea(graphArea)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

NodeVisulizer::~NodeVisulizer()
{
    qDebug() << "node deconstruct!";
}
