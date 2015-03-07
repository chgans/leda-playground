#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsScene;
class GraphicsTool;
class GraphicsObject;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent);
    ~GraphicsView();

    GraphicsScene *scene();
    void setScene(GraphicsScene *scene);

    GraphicsTool *tool();
    void setTool(GraphicsTool *tool);

    GraphicsObject *objectAt(const QPoint &pos) const;

signals:
    void mouseDoubleClicked(const QPoint &pos);

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

private:
    GraphicsTool *m_tool;
};

#endif // GRAPHICSVIEW_H
