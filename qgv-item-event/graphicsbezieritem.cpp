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

void GraphicsBezierItem::addPoint(const QPointF &pos)
{
    Q_ASSERT(m_path.elementCount() == handleCount());

    if (m_px.size() == 0) {
        GraphicsHandle *node = new GraphicsHandle(GraphicsHandle::MoveRole,
                                                  GraphicsHandle::CircleHandle,
                                                  pos, this);
        m_handleToElementIndex[node] = 0;
        m_px.append(pos.x());
        m_py.append(pos.y());
        updateSpline();
    }
    else if (!(qFuzzyCompare(m_px.last(), pos.x()) &&
               qFuzzyCompare(m_py.last(), pos.y()))) {

        int elementIndex = m_path.elementCount();

        m_px.append(pos.x());
        m_py.append(pos.y());
        m_c1x.append(0);
        m_c1y.append(0);
        m_c2x.append(0);
        m_c2y.append(0);

        updateSpline();

        QPointF c1Pos(m_c1x.back(), m_c1y.back());
        QPointF c2Pos(m_c2x.back(), m_c2y.back());
        GraphicsHandle *c1 = new GraphicsHandle(GraphicsHandle::MoveRole,
                                                GraphicsHandle::DiamondHandle,
                                                c1Pos, this);
        m_handleToElementIndex[c1] = elementIndex++;
        GraphicsHandle *c2 = new GraphicsHandle(GraphicsHandle::MoveRole,
                                                GraphicsHandle::DiamondHandle,
                                                c2Pos, this);
        m_handleToElementIndex[c2] = elementIndex++;
        GraphicsHandle *node = new GraphicsHandle(GraphicsHandle::MoveRole,
                                                  GraphicsHandle::CircleHandle,
                                                  pos, this);
        m_handleToElementIndex[node] = elementIndex++;
    }
    else {
        qWarning() << QString("Cannot add a cubic bezier to pos [%1, %2] which is equal to current pos")
                      .arg(pos.x()).arg(pos.y());
    }
    setShapeDirty();
    setBoundingRectDirty();
}

void GraphicsBezierItem::removePoint(int index)
{
#if 0
    int nbElt = m_path.elementCount();
    int nbPoints = pointCount();
    int eltIndex = index*3;

    Q_ASSERT(nbElt > 0);
    Q_ASSERT(index < nbPoints);
    Q_ASSERT(m_path.elementCount() == handles().count());

    QPainterPath path;
    if (index == 0) {
        if (m_path.elementCount() > 1) {
            path.moveTo(m_path.elementAt(3).x, m_path.elementAt(3).y);
        }
        m_path = path;
        m_handleToElementIndex.remove(removeHandle(0));
    }
    else if (index == nbPoints - 1) {
        m_path = copyPath(m_path, 0, index - 1);
        m_handleToElementIndex.remove(removeHandle(3*index));
        m_handleToElementIndex.remove(removeHandle(3*index - 1));
        m_handleToElementIndex.remove(removeHandle(3*index - 2));
    }
    //qDebug() << m_path;
    setShapeDirty();
    setBoundingRectDirty();
#endif
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

void GraphicsBezierItem::updateHandles()
{
    m_updatingHandles = true;
    handleAt(0)->setPos(m_px[0], m_py[0]);
    for (int i = 1; i < m_px.size(); i++) {
        qreal c1x = m_c1x[i-1];
        qreal c1y = m_c1y[i-1];
        qreal c2x = m_c2x[i-1];
        qreal c2y = m_c2y[i-1];
        qreal px  = m_px[i];
        qreal py  = m_py[i];
        handleAt(3*i-2)->setPos(c1x, c1y);
        handleAt(3*i-1)->setPos(c2x, c2y);
        handleAt(3*i)->setPos(px, py);
    }
    m_updatingHandles = false;
}

QList<QPointF> GraphicsBezierItem::points() const
{
    QList<QPointF> result;
    for (int i = 0; i < m_px.size(); i++)
        result.append(QPointF(m_px[i], m_py[i]));
    return result;
}

int GraphicsBezierItem::pointCount() const
{
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

void GraphicsBezierItem::handleMoved(const GraphicsHandle *point)
{
    if (m_updatingHandles)
        return;

    Q_ASSERT(m_path.elementCount() == handleCount());
    Q_ASSERT(m_handleToElementIndex.contains(point));

    qreal x = point->pos().x();
    qreal y = point->pos().y();

    int elementIndex = m_handleToElementIndex.value(point);
    if (elementIndex == 0) {
        m_px[0] = x;
        m_py[0] = y;
    }
    else {
        int type = (elementIndex) % 3;
        int i = elementIndex / 3;
        if (type == 0) {
            m_px[i] = x;
            m_py[i] = y;
        }
        else if (type == 1) {
            m_c1x[i] = x;
            m_c1y[i] = y;
        }
        else {
            m_c2x[i] = x;
            m_c2y[i] = y;
        }
    }

    updateSpline();
    updateHandles();
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
