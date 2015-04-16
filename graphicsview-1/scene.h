#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QSize>
#include <QList>

class DesignLayer;
class DesignLayerManager;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    explicit Scene(const QRectF & sceneRect, QObject * parent = 0);
    explicit Scene(qreal x, qreal y, qreal width, qreal height,
                   QObject * parent = 0);

public slots:

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void init();

    QPointF snapScenePos(QPointF pos);

    // The size of the cells in the grid.
    QSize m_cellSize;
};

#endif // SCENE_H
