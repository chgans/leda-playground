#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QList>
#include <QMap>

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
class PcbPalette;

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

    void addLayer(DesignLayer *layer);
    void removeLayer(DesignLayer *layer);
    void addLayers(const QList<DesignLayer *> &layers);
    void removeLayers(const QList<DesignLayer *> &layers);
    QList<DesignLayer *> layers();

    void setActiveLayer(DesignLayer *layer);
    DesignLayer *activeLayer();

    void setPalette(PcbPalette *palette);
    PcbPalette *palette() const;

    void addMaskingItem(QGraphicsItem *item);
    void removeMaskingItem(QGraphicsItem *item);
    void setMaskingItems(QList<QGraphicsItem*> items);
    QList<QGraphicsItem*> maskingItems();
    void resetMaskingItems();

    void setLayerDisplayMode(LayerDisplayMode mode);
    LayerDisplayMode layerDisplayMode() const;

    virtual void setScene(Scene *scene);
    virtual Scene *scene() const;

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
    void sceneAdded();
    void sceneRemoved();
    void layerAdded(DesignLayer *layer);
    void layerRemoved(DesignLayer *layer);
    void layerVisibilityChanged(int index, bool visible);
    void activeLayerChanged(int previousIndex, int currentIndex);

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

protected:
    void updateLayerDisplayModes();
    void updateLayerZValues();

    void wheelEvent(QWheelEvent *event);
    void drawForeground(QPainter * painter, const QRectF & rect);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:

    Scene *m_scene;
    QMap<int, DesignLayer *> m_indexToLayer;
    DesignLayer *m_activeLayer;
    LayerDisplayMode m_layerDisplayMode;
    PcbPalette *m_palette;

    int m_designInsightDelay;
    QTimer m_designInsightTimer;
    QGraphicsItem *m_designInsightItem;
    InsightConnectivityWidget *m_connectivity;

    QList<QGraphicsItem*> m_maskingItems;

    InsightLensWidget *m_lens;
    InsightHeadsUpWidget *m_headsUp;

    InsightPickListWidget *m_pickList;
    QGraphicsItem *m_pickedItem;

protected slots:
    void showDesignInsight();
    void hideDesignInsight();

    void onItemSelectedFromPickList(QGraphicsItem* item);

};

#endif // MAINVIEW_H
