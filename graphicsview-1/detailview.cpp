#include "detailview.h"

#include <QEvent>
#include <QMouseEvent>

#include <QDebug>

DetailView::DetailView(QWidget *parent) :
    QGraphicsView(parent),
    mObservedView(0)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(false);
}

void DetailView::setObservedView(QGraphicsView *view)
{
    if(mObservedView)
        mObservedView->viewport()->removeEventFilter(this);
    view->viewport()->installEventFilter(this);
    mObservedView = view;
}

void DetailView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //fitInView(0, 0, 100, 100, Qt::KeepAspectRatio);
}

bool DetailView::eventFilter(QObject *obj, QEvent *ev)
{
    if (mObservedView && obj == mObservedView->viewport() &&
            ev->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent*>(ev);
        QTransform t = mObservedView->viewportTransform();
        QPointF pos = t.inverted().map(e->pos());
        setTransform(t*10);
        centerOn(pos);
    }
    return QGraphicsView::eventFilter(obj, ev);
}
