#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QSize>
#include <QList>

class DesignLayer;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    explicit Scene(const QRectF & sceneRect, QObject * parent = 0);
    explicit Scene(qreal x, qreal y, qreal width, qreal height,
                   QObject * parent = 0);

    QList<DesignLayer *> layers() const;
    void setLayers(const QList<DesignLayer *> layers);
    // Convenience function
    int addLayer(const QString &name, const QColor &color);

    void activateLayer(int idx);
    void activateLayer(DesignLayer *layer);
    DesignLayer *activeLayer() const;

    void addItemToLayer(QGraphicsItem *item, int index);
    void addItemToLayer(QGraphicsItem *item, DesignLayer *layer);
    void addItemToActiveLayer(QGraphicsItem *item);

signals:
    void layersChanged();
    void activeLayerAboutToChange(DesignLayer *layer);
    void activeLayerChanged(DesignLayer *layer);

public slots:

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void init();
    QList <DesignLayer *> m_layers;
    DesignLayer *m_activeLayer;


    QPointF snapScenePos(QPointF pos);

    // The size of the cells in the grid.
    QSize mCellSize;
};

#endif // SCENE_H
