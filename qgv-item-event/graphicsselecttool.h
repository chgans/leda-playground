#ifndef GRAPHICSSELECTTOOL_H
#define GRAPHICSSELECTTOOL_H

#include "graphicstool.h"

#include <QPoint>

class GraphicsObject;
class GraphicsHandle;

class QRubberBand;

class GraphicsSelectTool : public GraphicsTool
{
    Q_OBJECT

public:
    GraphicsSelectTool(QObject *parent = 0);
    ~GraphicsSelectTool();

private:
    enum ToolState {
        HintState,
        OperationState
    };
    enum Operation {
        DragSelect,
        //ClickSelect,
        MoveItem,
        MoveHandle,
        CloneItem
    };
    ToolState m_state;
    Operation m_operation;

    QPoint m_mousePressPosition;
    GraphicsObject *m_item;
    QList<GraphicsObject *> m_items;
    QList<GraphicsObject *> m_phantomItems;
    GraphicsHandle *m_handle;
    QRubberBand *m_rubberBand;

    void updateCursor(QMouseEvent *event);
    void setOperation(Operation operation);

    // GraphicsTool interface
public:
    void setView(GraphicsView *view);
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    QDialog *optionDialog();
    virtual void activate();
    virtual void desactivate();

    // GraphicsTool interface
public slots:
    virtual void cancel();


    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif // GRAPHICSSELECTTOOL_H
