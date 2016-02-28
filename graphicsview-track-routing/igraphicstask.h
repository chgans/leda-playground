#ifndef IGRAPHICSTASK
#define IGRAPHICSTASK

#include <QObject>

class QGraphicsSceneMouseEvent;
class QKeyEvent;
class GraphicsView;
class QGraphicsScene;

class IGraphicsTask: public QObject
{
    Q_OBJECT

public:
    IGraphicsTask(QObject *parent = nullptr);
    virtual ~IGraphicsTask();

    void setView(GraphicsView *view);
    GraphicsView *view() const;
    QGraphicsScene *scene() const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    GraphicsView *m_view;
};

#endif // IGRAPHICSTASK

