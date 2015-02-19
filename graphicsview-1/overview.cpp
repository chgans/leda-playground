#include "overview.h"

#include <QMouseEvent>
#include <QDebug>

OverView::OverView(QWidget *parent) :
    QGraphicsView(parent),
    mObservedView(0),
    mMove(NoMove)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignCenter);
    setInteractive(false);
    setMouseTracking(true); // Get mouse move event when no button pressed
    //setBackgroundBrush(Qt::black); // TODO: remove to let the scene draw the background
}

void OverView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
}

void OverView::setObservedView(QGraphicsView *view)
{
    if(mObservedView)
        mObservedView->viewport()->removeEventFilter(this);
    view->viewport()->installEventFilter(this);
    mObservedView = view;
}

void OverView::getObservedRect()
{
    QTransform t = mObservedView->viewportTransform();
    QRectF r = mObservedView->viewport()->rect();
    mObservedRect = t.inverted().mapRect(r);
}

// All painting is done in scene coordinates. rect is the exposed rectangle
void OverView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    //painter->setBrush(Qt::white);
    //painter->drawRect(scene()->sceneRect());
}

// All painting is done in scene coordinates. rect is the exposed rectangle
void OverView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::red, 3.0/transform().m11()));
    painter->drawRect(mObservedRect);
}

void OverView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    fitInView(scene()->sceneRect().adjusted(-50, -50, 50, 50),
              Qt::KeepAspectRatio);
}

void OverView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QPointF pos = mapToScene(event->pos());
    qreal radius = 5.0/transform().m11();
    mLastPos = pos;

    if ((mObservedRect.topLeft() - pos).manhattanLength() < radius)
        mMove = MoveTopLeft;
    else if ((mObservedRect.topRight() - pos).manhattanLength() < radius)
        mMove = MoveTopRight;
    else if ((mObservedRect.bottomLeft() - pos).manhattanLength() < radius)
        mMove = MoveBottomLeft;
    else if ((mObservedRect.bottomRight() - pos).manhattanLength() < radius)
        mMove = MoveBottomRight;
    else if (mObservedRect.contains(pos))
        mMove = MoveRect;
    else
        mMove = NoMove;
}

void OverView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    qreal radius = 5.0/transform().m11();
    switch (mMove) {
    case NoMove:
        if ((mObservedRect.topLeft() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeFDiagCursor);
        else if ((mObservedRect.topRight() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeBDiagCursor);
        else if ((mObservedRect.bottomLeft() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeBDiagCursor);
        else if ((mObservedRect.bottomRight() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeFDiagCursor);
        else if (mObservedRect.contains(pos))
            setCursor(Qt::SizeAllCursor);
        else
            setCursor(QCursor());
        break;
    case MoveTopLeft:
        mObservedRect.setTopLeft(pos);
        forceRedraw();
        break;
    case MoveTopRight:
        mObservedRect.setTopRight(pos);
        forceRedraw();
        break;
    case MoveBottomLeft:
        mObservedRect.setBottomLeft(pos);
        forceRedraw();
        break;
    case MoveBottomRight:
        mObservedRect.setBottomRight(pos);
        forceRedraw();
        break;
    case MoveRect:
        QPointF delta = pos - mLastPos;
        mObservedView->centerOn(mObservedRect.translated(delta).center());
        break;
    }
    mLastPos = pos;
}

void OverView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    switch (mMove) {
    case MoveTopLeft:
    case MoveTopRight:
    case MoveBottomLeft:
    case MoveBottomRight:
        mObservedView->resetMatrix();
        mObservedView->resetTransform();
        mObservedView->fitInView(mObservedRect, Qt::KeepAspectRatio);
        break;
    case NoMove:
    case MoveRect:
        break;
    }
    mMove = NoMove;
    setCursor(QCursor());
}

bool OverView::eventFilter(QObject *obj, QEvent *ev)
{
    // TODO: not efficient, get something better?
    if (mObservedView && obj == mObservedView->viewport() && ev->type() == QEvent::Paint) {
        getObservedRect();
        forceRedraw();
    }
    return QGraphicsView::eventFilter(obj, ev);
}

void OverView::forceRedraw()
{
    // TODO: get something better?
    viewport()->update();
}
