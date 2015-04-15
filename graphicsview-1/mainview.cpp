#include "mainview.h"
#include "overview.h"
#include "detailview.h"
#include "insightlenswidget.h"
#include "insightheadsupwidget.h"
#include "insightconnectivitywidget.h"
#include "insightpicklistwidget.h"
#include "scene.h"
#include "designlayer.h"
#include "designlayermanager.h"

#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>
#include <QGraphicsOpacityEffect>
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
    m_pickedItem = 0;

    m_scene = nullptr;
    m_layerManager = DesignLayerManager::instance();

    m_layerDisplayMode = DisplayAllLayers;

    setMouseTracking(true);
    m_designInsightDelay = 2000;
    m_designInsightTimer.setInterval(m_designInsightDelay);
    m_designInsightTimer.setSingleShot(true);
    m_designInsightItem = 0;
    connect(&m_designInsightTimer, &QTimer::timeout,
            this, &MainView::showDesignInsight);

    m_connectivity = new InsightConnectivityWidget;
    m_connectivity->setMaximumSize(300, 300);
    m_connectivity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_lens = new InsightLensWidget(this);
    m_lens->setBuddyView(this);
    m_lens->setLensShape(InsightLensWidget::SquareLens);
    m_lens->setMouseTracking(true);
    m_lens->setEnabled(true);

    m_headsUp = new InsightHeadsUpWidget(this);
    m_headsUp->move(5, 5);
    enableHeadsUp(true);

    m_pickList = new InsightPickListWidget(this);
    m_pickList->hide();
    connect(m_pickList, &InsightPickListWidget::itemSelected,
            this, &MainView::onItemSelectedFromPickList);

}

void MainView::addMaskingItem(QGraphicsItem *item)
{
    if (!m_maskingItems.contains(item))
        m_maskingItems.append(item);
}

void MainView::removeMaskingItem(QGraphicsItem *item)
{
    if (m_maskingItems.contains(item))
        m_maskingItems.removeOne(item);
}

void MainView::setMaskingItems(QList<QGraphicsItem *> items)
{
    m_maskingItems = items;
}

QList<QGraphicsItem *> MainView::maskingItems()
{
    return m_maskingItems;
}

void MainView::resetMaskingItems()
{
    m_maskingItems.clear();
}

// TODO: LayerDisplayMode is a layer property, so just update all layers
void MainView::setLayerDisplayMode(MainView::LayerDisplayMode mode)
{
    if (m_layerDisplayMode == mode)
        return;

    m_layerDisplayMode = mode;

    updateSceneLayersEffect();
    invalidateScene(QRectF(), QGraphicsScene::ItemLayer);

    emit layerDisplayModeChanged(m_layerDisplayMode);
}

MainView::LayerDisplayMode MainView::layerDisplayMode() const
{
    return m_layerDisplayMode;
}

MainView::LayerDisplayMode MainView::cycleLayerDisplayMode()
{
    LayerDisplayMode mode = LayerDisplayMode(int(m_layerDisplayMode) + 1);

    if (mode == _EndDisplayMode)
        mode = _BeginDisplayMode;

    setLayerDisplayMode(mode);
    return layerDisplayMode();
}

void MainView::setScene(Scene *scene)
{
    if (m_scene) {
        m_scene->disconnect(this);
        m_lens->setBuddyView(nullptr);
        m_connectivity->setBuddyView(nullptr);
        m_headsUp->setBuddyView(nullptr);
        emit sceneRemoved();
    }

    m_scene = scene;

    if (m_scene == nullptr)
        return;

    QGraphicsView::setScene(scene);
    m_lens->setBuddyView(this);
    m_connectivity->setBuddyView(this);
    m_headsUp->setBuddyView(this);

    updateSceneLayersEffect();

    connect(m_scene, &Scene::activeLayerAboutToChange,
            this, &MainView::onActiveLayerAboutToChange);
    connect(m_scene, &Scene::activeLayerChanged,
            this, &MainView::onActiveLayerChanged);

    emit sceneAdded();
}

// Should we instead provide access to the design insights object and monitor
// their property changes?
bool MainView::headsUpEnabled() const
{
    return m_headsUp->isEnabled();
}

void MainView::enableHeadsUp(bool enabled)
{
    m_headsUp->setEnabled(enabled);
    m_headsUp->setVisible(enabled);
}

bool MainView::headsUpTrackingEnabled() const
{
    return m_headsUp->displayedItem(InsightHeadsUpWidget::CursorLocation);
}

void MainView::enableHeadsUpTracking(bool enabled)
{
    m_headsUp->setDisplayedItem(InsightHeadsUpWidget::CursorLocation, enabled);
    m_headsUp->setDisplayedItemHover(InsightHeadsUpWidget::CursorLocation, enabled);
}

void MainView::resetHeadsUpDeltaOrigin()
{
    m_headsUp->resetDeltaOrigin();
}

bool MainView::headsUpDeltaOriginEnabled() const
{
    return m_headsUp->displayedItem(InsightHeadsUpWidget::LastClickDelta);
}

void MainView::enableHeadsUpDeltaOrigin(bool enabled)
{
    m_headsUp->setDisplayedItem(InsightHeadsUpWidget::LastClickDelta, enabled);
    m_headsUp->setDisplayedItemHover(InsightHeadsUpWidget::LastClickDelta, enabled);
}

bool MainView::insightLensEnabled() const
{
    return m_lens->isLensEnabled();
}

void MainView::enableInsightLens(bool enabled)
{
    m_lens->setLensEnabled(enabled);
}

void MainView::shiftInsightLensToMouse()
{
    m_lens->moveLensToMousePosition();
}

void MainView::enableInsightLensTracking(bool enabled)
{
    m_lens->setMouseTracking(enabled);
}

bool MainView::insightLensMouseTrackingEnabled() const
{
    return m_lens->mouseTracking();
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

// FIXME: This currently affects *all* views. Other solutions are:
//  - Use IndirectPainting optimisation flag (deprecated, will likely go away int Qt 6)
//  - Items make use of the paint function's QWidget parameter
//    (They could ask if it should draw or not, which painting effect, ...)
void MainView::onActiveLayerAboutToChange(DesignLayer *layer)
{
    updateSceneLayerEffect(layer, false);
}

void MainView::onActiveLayerChanged(DesignLayer *layer)
{
    updateSceneLayerEffect(layer, true);
}

void MainView::enableInsightLensSingleLayerMode(bool enabled)
{
    // TODO
    Q_UNUSED(enabled);
}

void MainView::toggleInsightLensShape()
{
    m_lens->toggleLensShape();
}

void MainView::onLayersChanged()
{
    updateSceneLayersEffect();
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
    if (!m_maskingItems.isEmpty()) {
        QPainterPath path1;
        path1.addRect(rect);
        QPainterPath path2;
        foreach (QGraphicsItem *item, m_maskingItems) {
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

    // TODO: Better placement strategy
    // TODO: PickList population depends on layer display mode too
    if (items(event->pos()).size() > 1) {
        m_pickList->move(mapFromGlobal(QCursor::pos()));
        m_pickList->setPickList(scene(), items(event->pos()));
        m_pickList->show();
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    hideDesignInsight();
    m_designInsightTimer.stop();
    if (items(event->pos()).size() > 0) {
        m_designInsightItem = items(event->pos()).first();
        m_designInsightTimer.start();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    m_designInsightItem = 0;
    m_designInsightTimer.stop();
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

void MainView::updateSceneLayersEffect()
{
    foreach (DesignLayer *layer, m_layerManager->allLayers()) {
        updateSceneLayerEffect(layer, layer == m_scene->activeLayer());
    }
}

void MainView::updateSceneLayerEffect(DesignLayer *layer, bool isActive)
{
    if (isActive) {
        layer->setGraphicsEffect(nullptr);
        return;
    }

    switch (m_layerDisplayMode) {
    case DisplayAllLayers:
        layer->setGraphicsEffect(nullptr);
        break;
    case GreyscaleOtherLayers:
    {
        // TODO: This is not a real grayscale, it is a graying out
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(Qt::gray);
        effect->setStrength(0.75);
        layer->setGraphicsEffect(effect);
        break;
    }
    case MonochromeOtherLayers:
    {
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(Qt::gray);
        effect->setStrength(1.0);
        layer->setGraphicsEffect(effect);
        break;
    }
    case HideOtherLayers:
    {
        // TODO: Anything better?
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        effect->setOpacity(0);
        layer->setGraphicsEffect(effect);
        break;
    }
    default:
        // Not reached
        Q_ASSERT(false);
    }
}

void MainView::showDesignInsight()
{
    m_connectivity->move(QCursor::pos());
    m_connectivity->show();
}

void MainView::hideDesignInsight()
{
    m_connectivity->hide();
}

void MainView::onItemSelectedFromPickList(QGraphicsItem *item)
{
    m_pickedItem = item;
    scene()->clearSelection();
    item->setSelected(true);
    QCursor::setPos(mapToGlobal(mapFromScene(item->scenePos())));
    m_pickList->hide();
}
