#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsScene;
class GraphicsTool;
class GraphicsObject;
class GraphicsHandle;

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
    const GraphicsHandle *handleAt(const QPoint &pos) const;

    const GraphicsHandle *handleUnderMouse() const;
    GraphicsObject *objectUnderMouse() const;
    QPoint mousePosition() const;

signals:
    void leftMouseButtonPressed();
    void leftMouseButtonReleased();
    void ctlKeyPressed();
    void ctlKeyReleased();
    void majKeyPressed();
    void majKeyReleased();
    void escKeyPressed();
    void escKeyReleased();
    void hoverItemEntered();
    void hoverItemLeft();
    void hoverHandleEntered();
    void hoverHandleLeft();
    void mouseMoved();

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
    GraphicsObject *m_objectUnderMouse;
    const GraphicsHandle *m_handleUnderMouse;
};

#endif // GRAPHICSVIEW_H
