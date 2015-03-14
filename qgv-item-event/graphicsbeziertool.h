#ifndef GRAPHICSBEZIERTOOL_H
#define GRAPHICSBEZIERTOOL_H

#include "graphicstool.h"

#include <QPointF>

class GraphicsBezierItem;

class GraphicsBezierTool : public GraphicsTool
{
public:
    GraphicsBezierTool(QObject *parent);

private:
    QPointF mapToScene(const QPoint &pos);
    QPointF mapToItem(const QPoint &pos);
    QPointF mapFromScene(const QPointF &pos);
    QPointF mapFromItem(const QPointF &pos);

    enum State {
        NotStarted,
        FirstPoint,
        MidPoints,
        LastPoint
    };
    State m_state;
    void setState(State state);
    GraphicsBezierItem *m_item;
    QPoint m_nodePos;
    bool m_insertHandleOnMouseMove = true;

    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    virtual void activate();
    virtual void desactivate();
};

#endif // GRAPHICSBEZIERTOOL_H
