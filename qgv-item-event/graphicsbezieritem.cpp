#include "graphicsbezieritem.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicscontrolpoint.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

GraphicsBezierItem::GraphicsBezierItem(GraphicsObject *parent):
    GraphicsObject(parent)
{
    setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
}

QPainterPath GraphicsBezierItem::path() const
{
    return m_path;
}

void GraphicsBezierItem::setPath(const QPainterPath &path)
{
    m_path = path;
    setShapeDirty();
    setBoundingRectDirty();
}

QPen GraphicsBezierItem::pen() const
{
    return m_pen;
}

void GraphicsBezierItem::setPen(const QPen &pen)
{
    m_pen = pen;
    setShapeDirty();
    setBoundingRectDirty();
}

void GraphicsBezierItem::addPoint(const QPointF &pos)
{
    Q_ASSERT(m_path.elementCount() == controlPoints().count());

    if (m_path.elementCount() == 0) {
        const GraphicsControlPoint *node = addControlPoint(GraphicsControlPoint::MoveRole,
                                                           pos);
        m_handleToElementIndex[node] = 0;
        m_path.moveTo(node->pos());
    }
    else if (!(qFuzzyCompare(m_path.currentPosition().x(), pos.x()) &&
               qFuzzyCompare(m_path.currentPosition().x(), pos.x()))) {
        int elementIndex = m_path.elementCount();

        QPointF c1Pos = m_path.currentPosition();
        QPointF c2Pos = pos;
        const GraphicsControlPoint *c1 = addControlPoint(GraphicsControlPoint::MoveRole,
                                                         c1Pos);
        const GraphicsControlPoint *c2 = addControlPoint(GraphicsControlPoint::MoveRole,
                                                         c2Pos);
        const GraphicsControlPoint *node = addControlPoint(GraphicsControlPoint::MoveRole,
                                                           pos);

        m_handleToElementIndex[c1] = elementIndex++;
        m_handleToElementIndex[c2] = elementIndex++;
        m_handleToElementIndex[node] = elementIndex++;

        m_path.cubicTo(c1->pos(), c2->pos(), node->pos());
    }
    else {
        qWarning() << QString("Cannot add a cubic bezier to pos [%1, %2] which is equal to current pos")
                      .arg(pos.x()).arg(pos.y());
    }
    //qDebug() << m_path << controlPoints().count();
    setShapeDirty();
    setBoundingRectDirty();
}

void GraphicsBezierItem::removePoint(int index)
{
    int nbElt = m_path.elementCount();
    int nbPoints = pointCount();
    int eltIndex = index*3;

    Q_ASSERT(nbElt > 0);
    Q_ASSERT(index < nbPoints);
    Q_ASSERT(m_path.elementCount() == controlPoints().count());

    QPainterPath path;
    if (index == 0) {
        if (m_path.elementCount() > 1) {
            path.moveTo(m_path.elementAt(3).x, m_path.elementAt(3).y);
        }
        m_path = path;
        m_handleToElementIndex.remove(removeControlPoint(0));
    }
    else if (index == nbPoints - 1) {
        m_path = copyPath(m_path, 0, index - 1);
        m_handleToElementIndex.remove(removeControlPoint(3*index));
        m_handleToElementIndex.remove(removeControlPoint(3*index - 1));
        m_handleToElementIndex.remove(removeControlPoint(3*index - 2));
    }
    //qDebug() << m_path;
    setShapeDirty();
    setBoundingRectDirty();
}

QPainterPath GraphicsBezierItem::copyPath(const QPainterPath &src, int first, int last)
{
    QPainterPath dst;
    int i = first;
    while (i <= (last*3)) {
        if (i == 0) {
            QPainterPath::Element elt = src.elementAt(i);
            dst.moveTo(elt.x, elt.y);
            i++;
        }
        else {
            QPainterPath::Element c1 = src.elementAt(i);
            QPainterPath::Element c2 = src.elementAt(i+1);
            QPainterPath::Element pt = src.elementAt(i+2);
            dst.cubicTo(c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
            i+= 3;
        }
    }
    //qDebug() << dst;
    return dst;
}

QList<QPointF> GraphicsBezierItem::points() const
{
    QList<QPointF> result;
    QPainterPath::Element elt;
    int count = m_path.elementCount();
    int i = 0;
    while (i < count) {
        elt = m_path.elementAt(i);
        result.append(QPointF(elt.x, elt.y));
        if (i > 0)
            i += 3;
        else
            i++;
    }
    return result;
}

int GraphicsBezierItem::pointCount() const
{
    if (m_path.elementCount() < 2)
        return m_path.elementCount();
    int count = 1 + (m_path.elementCount() - 1) / 3;
    //qDebug() << "Count" << count;
    return count;
}

void GraphicsBezierItem::setBoundingRectDirty()
{
    prepareGeometryChange();
    m_boundingRectIsDirty = true;
}

void GraphicsBezierItem::computeBoundingRect() const
{
    qDebug() << __PRETTY_FUNCTION__;
    if (isSelected()) {
        // FIXME: get the real extra
        qreal extra = 2.5;
        m_boundingRect = m_path.controlPointRect().adjusted(-extra, -extra, +extra, +extra);
    }
    else {
        qreal extra = pen().widthF()/2.0;
        m_boundingRect = m_path.boundingRect().adjusted(-extra, -extra, +extra, +extra);
    }

    m_boundingRectIsDirty = false;
}

void GraphicsBezierItem::setShapeDirty()
{
    m_shapeIsDirty = true;
}

// TBD: Do we really need to include the handle path in shape?
void GraphicsBezierItem::computeShape() const
{
    qDebug() << __PRETTY_FUNCTION__;
    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.widthF());
    stroker.setCapStyle(m_pen.capStyle());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setMiterLimit(m_pen.miterLimit());
    if (isSelected()) {
        QPainterPath path;
        path = stroker.createStroke(m_path);
        m_shape = (path + controlPointsShape()).simplified(); // Performance killer!
    }
    else
        m_shape = stroker.createStroke(m_path);

    m_shapeIsDirty = false;
}

QRectF GraphicsBezierItem::boundingRect() const
{
    if (m_boundingRectIsDirty)
        computeBoundingRect();
    return m_boundingRect;
}

QPainterPath GraphicsBezierItem::shape() const
{
    if (m_shapeIsDirty)
        computeShape();
    return m_shape;
}

void GraphicsBezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // TBD: exposedRect seems to be always equal boudingRect()
    // http://thesmithfam.org/blog/2007/02/03/qt-improving-qgraphicsview-performance/
    if (!shape().intersects(option->exposedRect))
        return;

    // From same web page
    painter->setClipRect(option->exposedRect);

    painter->setPen(pen());
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(m_path);
    if (isSelected()) {
        painter->save();
        paintControlPoints(painter, option, widget);
        painter->restore();
        painter->setPen(QPen(Qt::white, 0, Qt::DashLine));
        painter->drawRect(m_boundingRect);
    }
}

GraphicsObject *GraphicsBezierItem::clone()
{
    GraphicsBezierItem *item = new GraphicsBezierItem();
    GraphicsObject::cloneTo(item);
    item->setPen(pen());
    item->m_path = m_path;
    return item;
}

// TODO:
// - when moving a node, move ctl points as well
// - ctl points can be symetric, move one -> move the other
// - use moveControlPoint(), beware of infinite recursion
void GraphicsBezierItem::controlPointMoved(const GraphicsControlPoint *point)
{
    Q_ASSERT(m_path.elementCount() == controlPoints().count());
    Q_ASSERT(m_handleToElementIndex.contains(point));

    int elementIndex = m_handleToElementIndex.value(point);
    m_path.setElementPositionAt(elementIndex, point->pos().x(), point->pos().y());

    QRectF r = point->boundingRect();
    r.moveCenter(point->pos());
    setShapeDirty();
    setBoundingRectDirty();
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    qDebug() << change;

    // Notify the scene that boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        setShapeDirty();
        setBoundingRectDirty();
    }
    return value;
}
