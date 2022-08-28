#ifndef NODEVISULIZER_H
#define NODEVISULIZER_H

#include <QGraphicsObject>
#include <QObject>
#include <QtWidgets>
#include <QList>

class EdgeVisulizer;
class MyGraphVisulizer;
class NodeVisulizer : public QGraphicsObject
{
    Q_OBJECT
public:
    using NodeEdgeArray = QList<std::pair<NodeVisulizer*, EdgeVisulizer*>>;
private:
    int uniqueID;
    QGraphicsTextItem *attachedLabel;
    QString informationInside, informationOutside;
    MyGraphVisulizer *graphArea;
    QColor nowColor;
    NodeEdgeArray outEdgeList, inEdgeList;
    QFont nowFont;
    const static int NODE_DIAM;
    const static QPoint NODE_TOPLEFT_CORNER;
    const static QPoint TEXT_TOPLEFT_CORNER;
    const static int NODE_RING_DIAM;

public:
//    using NodeEdgeArray = QList<std::pair<NodeVisulizer*, EdgeVisulizer*>>;
//    NodeVisulizer(MyGraphVisulizer *graphArea);
    NodeVisulizer(MyGraphVisulizer *graphArea, QGraphicsItem *parent = nullptr);
    ~NodeVisulizer();
    void addEdge(NodeVisulizer *v, EdgeVisulizer *edge);
    EdgeVisulizer * removeEdge(NodeVisulizer *v);
    EdgeVisulizer *isConnect(NodeVisulizer *v) const;
    const NodeEdgeArray &getEdges() const;
    NodeEdgeArray &getEdges();
    void changeColor(QColor color);
    void changeFont(QFont font);
    void setInsideInfo(const QString &str);
    QString getInsideInfo() const;
    void setOutsideInfo(const QString &str);
    int getID() const;
    void enableMove();
    void disableMove();
    int getUniqueID() const;
    void setUniqueID(int newUniqueID);
    // QGraphicsItem interface
public:
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    enum {Type = UserType + 1};

    int type() const override;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
signals:
    void nodeClicked(NodeVisulizer *);
    void nodeDestroyed(NodeVisulizer *, const QString&);


};

#endif // NODEVISULIZER_H
