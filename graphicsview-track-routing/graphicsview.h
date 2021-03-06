#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QEvent>

#include "igraphicstask.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = 0);

signals:

public slots:
    void zoomIn();
    void zoomOut();

private:
    IGraphicsTask *m_task;
    void buildMouseSceneEvent(QGraphicsSceneMouseEvent *sceneEvent, QMouseEvent *event);
    void scaleView(qreal scaleFactor);


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // GRAPHICSVIEW_H
