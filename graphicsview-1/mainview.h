#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QList>

class QResizeEvent;
class QGraphicsItem;
class OverView;
class DetailView;
class InsightLensWidget;
class InsightHeadsUpWidget;
class InsightConnectivityWidget;
class InsightPickListWidget;
class Scene;
class DesignLayer;
class DesignLayerManager;

// TODO: rename to Pcb2dView
//  Add flags to enable tools (lens, ...)

class MainView : public QGraphicsView
{
    Q_OBJECT

    Q_ENUMS(LayerDisplayMode)

    Q_PROPERTY(LayerDisplayMode layerDisplayMode READ layerDisplayMode WRITE setLayerDisplayMode NOTIFY layerDisplayModeChanged)

public:
    enum LayerDisplayMode {
        DisplayAllLayers,
        GreyscaleOtherLayers,
        MonochromeOtherLayers,
        HideOtherLayers,

        _BeginDisplayMode = DisplayAllLayers,
        _EndDisplayMode = HideOtherLayers + 1
    };

    explicit MainView(QWidget *parent = 0);

    void addMaskingItem(QGraphicsItem *item);
    void removeMaskingItem(QGraphicsItem *item);
    void setMaskingItems(QList<QGraphicsItem*> items);
    QList<QGraphicsItem*> maskingItems();
    void resetMaskingItems();

    void setLayerDisplayMode(LayerDisplayMode mode);
    LayerDisplayMode layerDisplayMode() const;

    // Needed for magnifier
    virtual void setScene(Scene *scene);

    bool headsUpEnabled() const;
    bool headsUpTrackingEnabled() const;
    bool headsUpDeltaOriginEnabled() const;
    bool insightLensEnabled() const;
    bool insightLensMouseTrackingEnabled() const;
    bool insightLensAutoZoomEnabled() const;
    bool insightLensSingleLayerEnabled() const;

signals:
    void viewportChanged();
    void layerDisplayModeChanged(LayerDisplayMode mode);

public slots:
    LayerDisplayMode cycleLayerDisplayMode();

    void enableHeadsUp(bool enabled);
    void enableHeadsUpTracking(bool enabled);
    void resetHeadsUpDeltaOrigin();
    void enableHeadsUpDeltaOrigin(bool enabled);
    void enableInsightLens(bool enabled);
    void shiftInsightLensToMouse();
    void enableInsightLensTracking(bool enabled);
    void enableInsightLensAutoZoom(bool enabled);
    void enableInsightLensSingleLayerMode(bool enabled);
    void toggleInsightLensShape();

protected slots:
    void onLayersChanged();
    void onActiveLayerAboutToChange(DesignLayer *layer);
    void onActiveLayerChanged(DesignLayer *layer);

protected:
    void wheelEvent(QWheelEvent *event);
    void drawForeground(QPainter * painter, const QRectF & rect);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void updateSceneLayersEffect();
    void updateSceneLayerEffect(DesignLayer *layer, bool isActive);

    Scene *m_scene;
    DesignLayerManager *m_layerManager;
    LayerDisplayMode m_layerDisplayMode;

    int mDesignInsightDelay;
    QTimer mDesignInsightTimer;
    QGraphicsItem *mDesignInsightItem;
    InsightConnectivityWidget *mConnectivity;

    QList<QGraphicsItem*> mMaskingItems;

    InsightLensWidget *mLens;
    InsightHeadsUpWidget *mHeadsUp;

    InsightPickListWidget *mPickList;
    QGraphicsItem *mPickedItem;

protected slots:
    void showDesignInsight();
    void hideDesignInsight();

    void onItemSelectedFromPickList(QGraphicsItem* item);

};

#endif // MAINVIEW_H
