#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QSize>
#include <QList>

class GSceneLayer;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    explicit Scene(const QRectF & sceneRect, QObject * parent = 0);
    explicit Scene(qreal x, qreal y, qreal width, qreal height,
                   QObject * parent = 0);

    QList<GSceneLayer *> layers() const;
    void setLayers(const QList<GSceneLayer *> layers);
    // Convenience function
    int addLayer(const QString &name, const QColor &color);

    void activateLayer(int idx);
    void activateLayer(GSceneLayer *layer);
    GSceneLayer *activeLayer() const;

    void addItemToLayer(QGraphicsItem *item);

signals:
    void layersChanged();
    void activeLayerchanged();

public slots:

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void init();
    QList <GSceneLayer *> m_layers;
    GSceneLayer *m_activeLayer;


    QPointF snapScenePos(QPointF pos);

    // The size of the cells in the grid.
    QSize mCellSize;
    // The item being dragged.
    QGraphicsItem *mDragged;
    // The distance from the top left of the item to the mouse position.
    QPointF mDragOffset;
};

#endif // SCENE_H
