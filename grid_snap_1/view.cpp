#include "view.h"
#include "gridmanager.h"
#include "igrid.h"

#include <QWheelEvent>
#include <QGLWidget>
#include <QLinearGradient>
#include <QtMath>

#include <QGraphicsItem>

#include <QPicture>
#include <QImage>

#include <QDebug>

View::View(QWidget *parent) :
    QGraphicsView(parent),
    mBoard(0), mPathItem(0),
    mAngleConstraint(AnyAngle),
    mCornerStyle(PlainCorner),
    mSwapStartEndAngles(false)
{
    mGridManager = GridManager::instance();
    setViewport(new QGLWidget);
    setMouseTracking(true);
    setTransformationAnchor(NoAnchor);
    setResizeAnchor(NoAnchor);

    QPainterPath p1;
    p1.arcTo(0, 0, 100, 100, 0, 180);
    QPainterPath p2;
    p2.moveTo(p1.currentPosition());
    p2.lineTo(50, 50);
    p2.lineTo(120, 30);
    mSubPath << p1 << p2;

    QPainterPath p;
    p.connectPath(p1);
    p.connectPath(p2);

    qDebug() << mSubPath;
    qDebug() << p;
}

void View::initialise()
{
    mBoard = new QGraphicsRectItem(200, 400, 400, 200);
    mBoard->setBrush(QBrush(Qt::black));
    mBoard->setPen(QPen(Qt::red, 1));
    scene()->addItem(mBoard);
}

void View::drawBackground(QPainter *painter, const QRectF &rect)
{
    //Debug() << __PRETTY_FUNCTION__ << rect << mapToScene(viewport()->rect());
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, Qt::darkBlue);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, QBrush(gradient));
    painter->setPen(QPen(Qt::white, 0, Qt::DashLine));
    painter->drawRect(sceneRect());
}

void View::drawForeground(QPainter *painter, const QRectF &rect)
{
    drawGrids(painter, rect);
    drawCursor(painter, rect);
    QGraphicsView::drawForeground(painter, rect);
}

void View::wheelEvent(QWheelEvent *event)
{

    if (event->modifiers() & Qt::ControlModifier &&
            event->orientation() == Qt::Vertical) {
        qreal angle = event->angleDelta().y();
        qreal factor = qPow(1.0015, angle);
        QPointF pos = mapToScene(event->pos());
        scale(factor, factor);
        pos -= mapToScene(event->pos());
        translate(-pos.x(), -pos.y());
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    QSizeF pixelSize = QSizeF(transform().m11(), transform().m22());

    setCursorPosition(QPointF());
    if (mBoard->contains(pos)) {
        // Test grids in reverse order (top most first)
        for (int i = mGridManager->grids().count() - 1; i >= 0; --i) {
            const IGrid *grid = mGridManager->grids().at(i);
            if (grid->shape(pixelSize).contains(pos)) {
                setCursorPosition(grid->snap(pixelSize, pos));
                event->accept();
                break;
            }
        }
    }
    if (mPathItem && !mSubPath.isEmpty())
        updateSubPath(mCursorPosition);
}

void View::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (event->button() == Qt::LeftButton) {
        if (!mPathItem)
            startPathItem(mCursorPosition);
        if (!mSubPath.isEmpty())
            finishSubPath(mCursorPosition);
        startSubPath(mCursorPosition);
    }
    else if (event->button() == Qt::RightButton) {
        if (mPathItem) {
            finishPathItem();
        }
    }
}

void View::keyPressEvent(QKeyEvent *event)
{

    // TBD:
    //  SHIFT+A: decrease radius by minor grid
    //  SHIFT+S: increase radius by major grid
    //
    // TBD: edit mode

    if (event->key() == Qt::Key_Z)
        cycleAngleConstraint();
    else if (event->key() == Qt::Key_X)
        toggleSwapStartEndAngles();
    else if (event->key() == Qt::Key_C)
        cycleCornerStyle();
}

void View::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void View::setCursorPosition(const QPointF &pos)
{
    mCursorPosition = pos;
    scene()->update();
}

void View::drawGrids(QPainter *painter, const QRectF &rect)
{
    // Top grids mask bottom grids, while the whole grid layer is transparent
    QSizeF pixelSize = QSizeF(transform().m11(), transform().m22());
    QList<QPainterPath> gridShapes;
    foreach (const IGrid* grid, mGridManager->grids()) {
        gridShapes.append(grid->shape(pixelSize));
    }
    foreach (const IGrid* grid, mGridManager->grids()) {
        gridShapes.takeFirst();
        if (grid->shape(pixelSize).intersects(rect)) {
            QPainterPath clipPath;
            clipPath.addRect(mBoard->boundingRect()); // FIXME: shape() ?
            foreach (QPainterPath path, gridShapes)
                clipPath = clipPath.subtracted(path);

            painter->setClipPath(clipPath);
            painter->save();
            grid->draw(pixelSize, painter, rect);
            painter->restore();
        }
    }
}

void View::drawCursor(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    if (mCursorPosition.isNull())
        return;
    qreal length = 50.0/transform().m11();
    QPointF top(mCursorPosition.x(),
                mCursorPosition.y() - length);
    QPointF bottom(mCursorPosition.x(),
               mCursorPosition.y() + length);
    QPointF right(mCursorPosition.x() + length,
               mCursorPosition.y());
    QPointF left(mCursorPosition.x() - length,
               mCursorPosition.y());
    painter->setPen(QPen(Qt::white, 0, Qt::SolidLine));
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
}

void View::cycleAngleConstraint()
{
    mAngleConstraint = static_cast<AngleConstraint>((mAngleConstraint+1)%4);
    updateSubPath(mCursorPosition);
}

void View::cycleCornerStyle()
{
    mCornerStyle = static_cast<CornerStyle>((mCornerStyle+1)%3);
    updateSubPath(mCursorPosition);
}

void View::toggleSwapStartEndAngles()
{
    mSwapStartEndAngles = !mSwapStartEndAngles;
    updateSubPath(mCursorPosition);
}

void View::startPathItem(QPointF point)
{
    mPathItem = new QGraphicsPathItem;
    mPathItem->setPos(point);
    QPen pen(Qt::magenta, 0, Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    mPathItem->setPen(pen);
    mPathItem->setBrush(QBrush(Qt::darkMagenta));
    scene()->addItem(mPathItem);
    mSubPath.clear();
}

void View::updatePathItem()
{
    QPainterPath path;
    foreach (QPainterPath p, mSubPath) {
        path.connectPath(p);
    }
    path.closeSubpath();
    mPathItem->setPath(path);
}

void View::finishPathItem()
{
    if (mSubPath.size() <= 1) {
        scene()->removeItem(mPathItem);
        delete mPathItem;
    }
    else {
        mSubPath.removeLast();
        updatePathItem();
    }
    mSubPath.clear();
    mPathItem = 0;
}

void View::startSubPath(QPointF point)
{
    QPainterPath path;
    if (!mSubPath.isEmpty())
        path.moveTo(mSubPath.last().currentPosition());
    else
        path.moveTo(0, 0);

    mSubPath << path;
}

void View::updateSubPath(QPointF endPoint)
{
    if (!mPathItem || endPoint.isNull() || mSubPath.isEmpty())
        return;
    // TODO: check for start == end
    // TODO: check if the 3 points are aligned
    // TODO: check if corner fits (dynamic radius)
    // TODO: simplify qpainterpath

    QPointF p0(mSubPath.last().pointAtPercent(0));
    QPointF p1, p2;
    QPointF p3(mPathItem->mapFromScene(endPoint));
    qreal a0, a3;

    if (p0 == p3)
        return;

    mSubPath.removeLast();
    QPainterPath subpath;
    subpath.moveTo(p0);

    if (mAngleConstraint == AnyAngle) {
        subpath.lineTo(p3);
    }
    else {
        qreal startAngle, endAngle;
        qreal radius = 5.0;
        if (mAngleConstraint == RightAngle) {
            startAngle = 0;
            endAngle = 90;
        }
        else if (mAngleConstraint == FDiagAngle) {
            startAngle = 0;
            endAngle = 45;
        }
        else if (mAngleConstraint == BDiagAngle) {
            startAngle = 0;
            endAngle = -45;
        }
        if (!mSwapStartEndAngles) {
            a0 = startAngle;
            a3 = endAngle;
        }
        else {
            a0 = endAngle;
            a3 = startAngle;
        }

        qreal length = (p3-p0).manhattanLength();
        QLineF l0 = QLineF::fromPolar(length, a0).translated(p0);
        QLineF l3 = QLineF::fromPolar(length, a3).translated(p3);
        QPointF px;
        QLineF::IntersectType xtype = l0.intersect(l3, &px);
        Q_ASSERT(xtype != QLineF::NoIntersection);

        QLineF l1(px, p0);
        l1.setLength(radius);
        p1 = l1.p2();

        QLineF l2(px, p3);
        l2.setLength(radius);
        p2 = l2.p2();

        if (mCornerStyle == PlainCorner) {
            subpath.lineTo(px);
            subpath.lineTo(p3);
        }
        else if ( mCornerStyle == MitterCorner){
            subpath.lineTo(p1);
            subpath.lineTo(p2);
            subpath.lineTo(p3);
        }
        else if ( mCornerStyle == ArcCorner){
            subpath.lineTo(p1);
            subpath.quadTo(px, p2);
            subpath.lineTo(p3);
        }
    }

    qDebug() << subpath;
    mSubPath << subpath;

    updatePathItem();
}

void View::finishSubPath(QPointF point)
{
    updateSubPath(point);
}
