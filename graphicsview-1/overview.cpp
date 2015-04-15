#include "overview.h"

#include <QMouseEvent>
#include <QDebug>

OverView::OverView(QWidget *parent) :
    QGraphicsView(parent),
    m_observedView(nullptr),
    m_move(NoMove)
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
    if (m_observedView && m_observedView->viewport())
        m_observedView->viewport()->removeEventFilter(this);
    m_observedView = view;

    if (m_observedView && m_observedView->scene())
        setScene(m_observedView->scene());
}

void OverView::getObservedRect()
{
    QTransform t = m_observedView->viewportTransform();
    QRectF r = m_observedView->viewport()->rect();
    m_observedRect = t.inverted().mapRect(r);
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
    painter->drawRect(m_observedRect);
}

void OverView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (m_observedView && scene())
        fitInView(scene()->sceneRect().adjusted(-50, -50, 50, 50),
                  Qt::KeepAspectRatio);
}

void OverView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QPointF pos = mapToScene(event->pos());
    qreal radius = 5.0/transform().m11();
    m_lastPos = pos;

    if ((m_observedRect.topLeft() - pos).manhattanLength() < radius)
        m_move = MoveTopLeft;
    else if ((m_observedRect.topRight() - pos).manhattanLength() < radius)
        m_move = MoveTopRight;
    else if ((m_observedRect.bottomLeft() - pos).manhattanLength() < radius)
        m_move = MoveBottomLeft;
    else if ((m_observedRect.bottomRight() - pos).manhattanLength() < radius)
        m_move = MoveBottomRight;
    else if (m_observedRect.contains(pos))
        m_move = MoveRect;
    else
        m_move = NoMove;
}

void OverView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    qreal radius = 5.0/transform().m11();
    switch (m_move) {
    case NoMove:
        if ((m_observedRect.topLeft() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeFDiagCursor);
        else if ((m_observedRect.topRight() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeBDiagCursor);
        else if ((m_observedRect.bottomLeft() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeBDiagCursor);
        else if ((m_observedRect.bottomRight() - pos).manhattanLength() < radius)
            setCursor(Qt::SizeFDiagCursor);
        else if (m_observedRect.contains(pos))
            setCursor(Qt::SizeAllCursor);
        else
            setCursor(QCursor());
        break;
    case MoveTopLeft:
        m_observedRect.setTopLeft(pos);
        forceRedraw();
        break;
    case MoveTopRight:
        m_observedRect.setTopRight(pos);
        forceRedraw();
        break;
    case MoveBottomLeft:
        m_observedRect.setBottomLeft(pos);
        forceRedraw();
        break;
    case MoveBottomRight:
        m_observedRect.setBottomRight(pos);
        forceRedraw();
        break;
    case MoveRect:
        QPointF delta = pos - m_lastPos;
        m_observedView->centerOn(m_observedRect.translated(delta).center());
        break;
    }
    m_lastPos = pos;
}

void OverView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    switch (m_move) {
    case MoveTopLeft:
    case MoveTopRight:
    case MoveBottomLeft:
    case MoveBottomRight:
        m_observedView->resetMatrix();
        m_observedView->resetTransform();
        m_observedView->fitInView(m_observedRect, Qt::KeepAspectRatio);
        break;
    case NoMove:
    case MoveRect:
        break;
    }
    m_move = NoMove;
    setCursor(QCursor());
}

bool OverView::eventFilter(QObject *obj, QEvent *ev)
{
    // TODO: not efficient, get something better?
    if (m_observedView && obj == m_observedView->viewport() && ev->type() == QEvent::Paint) {
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


void OverView::showEvent(QShowEvent *event)
{
    if (m_observedView != nullptr)
        m_observedView->viewport()->installEventFilter(this);
}
