#include "detailview.h"

#include <QEvent>
#include <QMouseEvent>

#include <QDebug>

DetailView::DetailView(QWidget *parent) :
    QGraphicsView(parent),
    m_observedView(0)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(false);
}

void DetailView::setObservedView(QGraphicsView *view)
{
    if(m_observedView)
        m_observedView->viewport()->removeEventFilter(this);
    view->viewport()->installEventFilter(this);
    m_observedView = view;
    setScene(m_observedView->scene());
}

void DetailView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //fitInView(0, 0, 100, 100, Qt::KeepAspectRatio);
}

bool DetailView::eventFilter(QObject *obj, QEvent *ev)
{
    if (m_observedView && obj == m_observedView->viewport() &&
            ev->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent*>(ev);
        QTransform t = m_observedView->viewportTransform();
        QPointF pos = t.inverted().map(e->pos());
        setTransform(t*10);
        centerOn(pos);
    }
    return QGraphicsView::eventFilter(obj, ev);
}
