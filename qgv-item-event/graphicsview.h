#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent);
    ~GraphicsView();

protected:


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

    void wheelEvent(QWheelEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    QGraphicsItem *createPhantomItem(QGraphicsItem *item);
    QList<QGraphicsItem *> createPhantomItems(QList<QGraphicsItem *> items);

private:
    struct MouseData;
    MouseData *m_mouseData;

};

#endif // GRAPHICSVIEW_H
