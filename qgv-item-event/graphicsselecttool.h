#ifndef GRAPHICSSELECTTOOL_H
#define GRAPHICSSELECTTOOL_H

#include "graphicstool.h"

#include <QPoint>

class GraphicsObject;
class GraphicsControlPoint;

class QStateMachine;
class QState;
class QSignalTransition;

class QRubberBand;

class GraphicsSelectTool : public GraphicsTool
{
    Q_OBJECT

public:
    GraphicsSelectTool(QObject *parent = 0);
    ~GraphicsSelectTool();

private slots:
    void handleMouseMove();

private:
    QPoint m_mousePressPosition;
    GraphicsObject *m_item;
    QList<GraphicsObject *> m_items;
    const GraphicsControlPoint *m_handle;
    QRubberBand *m_rubberBand;

    void buildStateMachine();
    void destroyStateMachine();
    QStateMachine *m_stateMachine;
    QState *m_topState;
    QState *m_operationStateGroup;
    QState *m_dragSelectState;
    QState *m_moveItemState;
    QState *m_cloneItemState;
    QState *m_moveHandleState;
    QState *m_stageStateGroup;
    QState *m_maybeState;
    QState *m_confirmedState;
    QState *m_startedState;
    QSignalTransition *m_startedTransition;

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

};

#endif // GRAPHICSSELECTTOOL_H
