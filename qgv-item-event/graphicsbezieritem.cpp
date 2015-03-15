#include "graphicsbezieritem.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicshandle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

/*
 * TODO:
 *  - Add a beginMove/endMove, while moving we don't have to recompute shape
 *  - implement removePoint
 *  - implement clone
 */

GraphicsBezierItem::GraphicsBezierItem(GraphicsObject *parent):
    GraphicsObject(parent), m_updatingHandles(false)
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

// pos is in local coordinate
void GraphicsBezierItem::addPoint(const QPointF &pos)
{
    m_updatingHandles = true;

    if (m_px.size() == 0) {
        GraphicsPathPoint *point = new GraphicsPathPoint(this);
        point->setNodePos(pos);
        point->setFirst(true);
        point->setLast(true);
        addObservedItem(point);

        qDebug() << "addPoint" << pos;
        m_px.append(pos.x());
        m_py.append(pos.y());
    }
    else if (!(qFuzzyCompare(m_px.last(), pos.x()) &&
               qFuzzyCompare(m_py.last(), pos.y()))) {

        // Update the dirst/last status of the ex-last one
        points().last()->setLast(false);

        // Same with the new last one
        GraphicsPathPoint *point = new GraphicsPathPoint(this);
        point->setLast(true);
        addObservedItem(point);

        // Add new point to spline
        qDebug() << "addPoint" << pos;
        m_px.append(pos.x());
        m_py.append(pos.y());
        m_c1x.append(0);
        m_c1y.append(0);
        m_c2x.append(0);
        m_c2y.append(0);

        // And update the spline control points coordinates
        updateSpline();

        // And the force the handles to use them
        updateHandles();
    }
    else {
        qWarning() << QString("Cannot add a cubic bezier to pos [%1, %2] which is equal to current pos")
                      .arg(pos.x()).arg(pos.y());
    }
    m_updatingHandles = false;
    setShapeDirty();
    setBoundingRectDirty();
}

void GraphicsBezierItem::removePoint(int index)
{
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

void GraphicsBezierItem::updateSpline()
{
    setShapeDirty();
    setBoundingRectDirty();

    if (m_px.size() >= 2) {
        computeControlPoints(m_px, m_c1x, m_c2x);
        computeControlPoints(m_py, m_c1y, m_c2y);
    }

    // TODO: don't always rebuild the whole path
    // If the pointCount() hasn't change
    m_path = QPainterPath();
    m_path.moveTo(m_px[0], m_py[0]);
    for (int i = 1; i < m_px.size(); i++) {
        qreal c1x = m_c1x[i-1];
        qreal c1y = m_c1y[i-1];
        qreal c2x = m_c2x[i-1];
        qreal c2y = m_c2y[i-1];
        qreal px  = m_px[i];
        qreal py  = m_py[i];
        m_path.cubicTo(c1x, c1y, c2x, c2y, px, py);
    }
}

// https://www.particleincell.com/2012/bezier-splines/
void GraphicsBezierItem::computeControlPoints(const QVector<qreal> &p, QVector<qreal> &c1, QVector<qreal> &c2)
{
    int n = p.size() - 1;
    int i;
    QVector<qreal> a(n), b(n), c(n), r(n);

    /* left most segment */
    a[0] = 0;
    b[0] = 2;
    c[0] = 1;
    r[0] = p[0] + 2*p[1];

    /* internal segments */
    for (i = 1; i < n - 1; i++)
    {
        a[i] = 1;
        b[i] = 4;
        c[i] = 1;
        r[i] = 4*p[i] + 2*p[i+1];
    }

    /* right segment */
    a[n-1] = 2;
    b[n-1] = 7;
    c[n-1] = 0;
    r[n-1] = 8*p[n-1] + p[n];

    /* solves Ax=b with the Thomas algorithm (from Wikipedia) */
    for (i = 1; i < n; i++)
    {
        qreal m = a[i] / b[i-1];
        b[i] = b[i] - m*c[i-1];
        r[i] = r[i] - m*r[i-1];
    }

    /* compute p1 */
    c1[n-1] = r[n-1]/b[n-1];
    for (i = n-2; i >= 0; --i)
        c1[i] = (r[i] - c[i] * c1[i+1]) / b[i];

    /* we have p1, now compute p2 */
    for (i = 0; i < n-1; i++)
        c2[i] = 2*p[i+1] - c1[i+1];

    c2[n-1] = 0.5*(p[n] + c1[n-1]);
}

//
void GraphicsBezierItem::updateHandles()
{
    Q_ASSERT(m_px.count() == childItems().count());

    m_updatingHandles = true;
    qDebug() << "updateHandles" << m_px.count();

    int i = 0;

    Q_ASSERT(pointAt(i)->isFirst());
    Q_ASSERT(!pointAt(i)->isLast() || m_px.size() == 1);
    pointAt(i)->setNodePos(m_px[i], m_py[i]);
    pointAt(i)->setControl2Pos(m_c1x[i], m_c1y[i]); // right
    i++;
    while (i < m_px.size() - 1) {
        Q_ASSERT(!pointAt(i)->isFirst());
        Q_ASSERT(!pointAt(i)->isLast());
        pointAt(i)->setNodePos(m_px[i], m_py[i]);
        pointAt(i)->setControl1Pos(m_c2x[i-1], m_c2y[i-1]); // left
        pointAt(i)->setControl2Pos(m_c1x[i], m_c1y[i]); // right
        i++;
    }
    Q_ASSERT(!pointAt(i)->isFirst() || m_px.size() == 1);
    Q_ASSERT(pointAt(i)->isLast());
    pointAt(i)->setNodePos(m_px[i], m_py[i]);
    pointAt(i)->setControl1Pos(m_c2x[i-1], m_c2y[i-1]); // left

    m_updatingHandles = false;
    qDebug() << "updateHandles" << m_updatingHandles;
}

GraphicsPathPoint *GraphicsBezierItem::pointAt(int idx)
{
    Q_ASSERT(idx < childItems().count());
    Q_ASSERT(m_px.count() == childItems().count());
    return dynamic_cast<GraphicsPathPoint *>(childItems().at(idx));
}

QList<GraphicsPathPoint *> GraphicsBezierItem::points() const
{
    //Q_ASSERT(m_px.count() == childItems().count());
    QList<GraphicsPathPoint*> result;
    foreach (QGraphicsItem *item, childItems()) {
        result.append(dynamic_cast<GraphicsPathPoint *>(item));
    }
    return result;
}


int GraphicsBezierItem::pointCount() const
{
    //Q_ASSERT(m_px.count() == childItems().count());
    return m_px.size();
}

void GraphicsBezierItem::setBoundingRectDirty()
{
    prepareGeometryChange();
    m_boundingRectIsDirty = true;
}

void GraphicsBezierItem::computeBoundingRect() const
{
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = m_path.boundingRect().adjusted(-extra, -extra, +extra, +extra);
    m_boundingRectIsDirty = false;
}

void GraphicsBezierItem::setShapeDirty()
{
    m_shapeIsDirty = true;
}

void GraphicsBezierItem::computeShape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.widthF());
    stroker.setCapStyle(m_pen.capStyle());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setMiterLimit(m_pen.miterLimit());
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
}

GraphicsObject *GraphicsBezierItem::clone()
{
    GraphicsBezierItem *item = new GraphicsBezierItem();
    GraphicsObject::cloneTo(item);
    item->setPen(pen());
    item->m_path = m_path;
    return item;
}

void GraphicsBezierItem::itemNotification(IGraphicsObservableItem *item)
{
    qDebug() << "itemNotification";
    if (m_updatingHandles)
        return;

    GraphicsPathPoint *point = dynamic_cast<GraphicsPathPoint*>(item);
    Q_ASSERT(point);

    qDebug() << point << point->nodePos() << point->control1Pos() << point->control2Pos();

    int pointIndex = points().indexOf(point);

    m_px[pointIndex] = point->nodePos().x();
    m_py[pointIndex] = point->nodePos().y();

    if (!point->isFirst()) {
        m_c2x[pointIndex-1] = point->control1Pos().x();
        m_c2y[pointIndex-1] = point->control1Pos().y();
    }

    if (!point->isLast()) {
        m_c1x[pointIndex] = point->control2Pos().x();
        m_c1y[pointIndex] = point->control2Pos().y();
    }

    if (m_px.size() >= 2) {
        updateSpline();
        updateHandles();
    }
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}
