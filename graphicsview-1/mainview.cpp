#include "mainview.h"
#include "overview.h"
#include "detailview.h"
#include "insightlenswidget.h"
#include "insightheadsupwidget.h"
#include "insightconnectivitywidget.h"
#include "insightpicklistwidget.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QToolTip>

MainView::MainView(QWidget *parent) :
    QGraphicsView(parent)
{
    mPickedItem = 0;

    setMouseTracking(true);
    mDesignInsightDelay = 2000;
    mDesignInsightTimer.setInterval(mDesignInsightDelay);
    mDesignInsightTimer.setSingleShot(true);
    mDesignInsightItem = 0;
    connect(&mDesignInsightTimer, SIGNAL(timeout()),
            this, SLOT(showDesignInsight()));

    mConnectivity = new InsightConnectivityWidget;
    mConnectivity->setMaximumSize(300, 300);
    mConnectivity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mLens = new InsightLensWidget(this);
    mLens->setBuddyView(this);
    mLens->setLensShape(InsightLensWidget::SquareLens);
    mLens->setMouseTracking(true);
    mLens->setEnabled(true);

    mHeadsUp = new InsightHeadsUpWidget(this);
    mHeadsUp->move(5, 5);
    enableHeadsUp(true);

    mPickList = new InsightPickListWidget(this);
    mPickList->hide();
    connect(mPickList, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(onItemSelectedFromPickList(QGraphicsItem*)));

}

void MainView::addMaskingItem(QGraphicsItem *item)
{
    if (!mMaskingItems.contains(item))
        mMaskingItems.append(item);
}

void MainView::removeMaskingItem(QGraphicsItem *item)
{
    if (mMaskingItems.contains(item))
        mMaskingItems.removeOne(item);
}

void MainView::setMaskingItems(QList<QGraphicsItem *> items)
{
    mMaskingItems = items;
}

QList<QGraphicsItem *> MainView::maskingItems()
{
    return mMaskingItems;
}

void MainView::resetMaskingItems()
{
    mMaskingItems.clear();
}

void MainView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
    mLens->setBuddyView(this);
    mConnectivity->setBuddyView(this);
    mHeadsUp->setBuddyView(this);
}

// Should we instead provide access to the design insights object and monitor
// their property changes?
bool MainView::headsUpEnabled() const
{
    return mHeadsUp->isEnabled();
}

void MainView::enableHeadsUp(bool enabled)
{
    mHeadsUp->setEnabled(enabled);
    mHeadsUp->setVisible(enabled);
}

bool MainView::headsUpTrackingEnabled() const
{
    return mHeadsUp->displayedItem(InsightHeadsUpWidget::CursorLocation);
}

void MainView::enableHeadsUpTracking(bool enabled)
{
    mHeadsUp->setDisplayedItem(InsightHeadsUpWidget::CursorLocation, enabled);
    mHeadsUp->setDisplayedItemHover(InsightHeadsUpWidget::CursorLocation, enabled);
}

void MainView::resetHeadsUpDeltaOrigin()
{
    mHeadsUp->resetDeltaOrigin();
}

bool MainView::headsUpDeltaOriginEnabled() const
{
    return mHeadsUp->displayedItem(InsightHeadsUpWidget::LastClickDelta);
}

void MainView::enableHeadsUpDeltaOrigin(bool enabled)
{
    mHeadsUp->setDisplayedItem(InsightHeadsUpWidget::LastClickDelta, enabled);
    mHeadsUp->setDisplayedItemHover(InsightHeadsUpWidget::LastClickDelta, enabled);
}

bool MainView::insightLensEnabled() const
{
    return mLens->isLensEnabled();
}

void MainView::enableInsightLens(bool enabled)
{
    mLens->setLensEnabled(enabled);
}

void MainView::shiftInsightLensToMouse()
{
    mLens->moveLensToMousePosition();
}

void MainView::enableInsightLensTracking(bool enabled)
{
    mLens->setMouseTracking(enabled);
}

bool MainView::insightLensMouseTrackingEnabled() const
{
    return mLens->mouseTracking();
}

bool MainView::insightLensAutoZoomEnabled() const
{
    // TODO
    return false;
}

void MainView::enableInsightLensAutoZoom(bool enabled)
{
    // TODO
    Q_UNUSED(enabled);
}

bool MainView::insightLensSingleLayerEnabled() const
{
    return false;
}

void MainView::enableInsightLensSingleLayerMode(bool enabled)
{
    // TODO
    Q_UNUSED(enabled);
}

void MainView::toggleInsightLensShape()
{
    mLens->toggleLensShape();
}

void MainView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        qreal factor;
        if (event->delta() > 0)
            factor = 1.2;
        else
            factor = 0.8;
        QMatrix m = matrix();
        m.scale(factor, factor);
        setMatrix(m);
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void MainView::drawForeground(QPainter *painter, const QRectF &rect)
{
    // Instead of dimming out all other components (decrease their opacity)
    // We grey them out with a semi-transparent mask
    if (!mMaskingItems.isEmpty()) {
        QPainterPath path1;
        path1.addRect(rect);
        QPainterPath path2;
        foreach (QGraphicsItem *item, mMaskingItems) {
            qDebug() << __FUNCTION__ << item->boundingRect();
            QRectF r = item->boundingRect().adjusted(-1, -1, 1, 1);
            r.translate(item->pos());
            path2.addRect(r);
        }
        QColor color = Qt::lightGray;
        color.setAlpha(128);
        painter->fillPath(path1.subtracted(path2), QBrush(color));
    }
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "There are" << items(event->pos()).size()
             << "items at position" << mapToScene(event->pos());

    if (items(event->pos()).size() > 1) {
        mPickList->move(mapFromGlobal(QCursor::pos())); // TODO: Better placement strategy
        mPickList->setPickList(scene(), items(event->pos()));
        mPickList->show();
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    mLastMousePos = event->pos();
    hideDesignInsight();
    mDesignInsightTimer.stop();
    if (items(event->pos()).size() > 0) {
        mDesignInsightItem = items(event->pos()).first();
        mDesignInsightTimer.start();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    mDesignInsightItem = 0;
    mDesignInsightTimer.stop();
    QGraphicsView::focusOutEvent(event);
}

void MainView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

bool MainView::eventFilter(QObject *obj, QEvent *event)
{
    return QGraphicsView::eventFilter(obj, event);
}

void MainView::showDesignInsight()
{
    mConnectivity->move(QCursor::pos());
    mConnectivity->show();
}

void MainView::hideDesignInsight()
{
    mConnectivity->hide();
}

void MainView::onItemSelectedFromPickList(QGraphicsItem *item)
{
    mPickedItem = item;
    scene()->clearSelection();
    item->setSelected(true);
    QCursor::setPos(mapToGlobal(mapFromScene(item->scenePos())));
    mPickList->hide();
}
