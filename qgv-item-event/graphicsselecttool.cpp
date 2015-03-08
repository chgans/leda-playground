#include "graphicsselecttool.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsobject.h"
#include "graphicscontrolpoint.h"
#include <QGraphicsItem>
#include <QGraphicsEffect>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QRubberBand>
#include <QAction>
#include <QDialog>

#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QSignalTransition>

#include <QDebug>

// Select tool:
//  Click based:
//  - clear selection: left press on background, release
//  - add/remove item to selection: left press on item w or w/o Shift modifier, release
//  - double left click => send signal
//  - right click => send signal (done by QWidget)
//  Drag based:
//  - drag select: left press on background, drag mouse, release
//  - single move: left press on item, drag mouse, release
//  - group move: left press on any selected items, drag mouse, release
//  - clone: left press w/ Ctrl on item, drag mouse, release
//  Cursors:
//   Qt::DragMoveCursor
//   Qt::DragCopyCursor
//   Qt::SizeAllCursor

GraphicsSelectTool::GraphicsSelectTool(QObject *parent):
    GraphicsTool(parent),
    m_mousePressPosition(QPoint(0, 0)),
    m_item(nullptr),
    m_handle(nullptr),
    m_rubberBand(new QRubberBand(QRubberBand::Rectangle))
{
}

GraphicsSelectTool::~GraphicsSelectTool()
{

}

QDialog *GraphicsSelectTool::optionDialog()
{
    return nullptr;
}

void GraphicsSelectTool::activate()
{
    m_stateMachine->start();
}

void GraphicsSelectTool::desactivate()
{
    m_stateMachine->stop();
}

void GraphicsSelectTool::handleMouseMove()
{
    if (m_dragSelectState->active()) {
        QRect viewRect = QRect(m_mousePressPosition, view()->mousePosition()).normalized();
        m_rubberBand->setGeometry(viewRect);
        qDebug() << "Rubberband" << viewRect;
        QPainterPath path;
        path.addPolygon(view()->mapToScene(viewRect));
        view()->scene()->setSelectionArea(path, Qt::IntersectsItemShape);
    }
    else if (m_moveItemState->active()) {

    }
    else if (m_cloneItemState->active()) {

    }
    else if (m_moveHandleState->active()) {

    }
}

void GraphicsSelectTool::buildStateMachine()
{
    m_stateMachine = new QStateMachine(this);
    m_topState = new QState(QState::ParallelStates);
    m_topState->addTransition(view(), SIGNAL(escKeyReleased()), m_topState);
    m_stateMachine->addState(m_topState);
    m_stateMachine->setInitialState(m_topState);

    m_operationStateGroup = new QState(m_topState);
    m_dragSelectState = new QState(m_operationStateGroup);
    m_operationStateGroup->setInitialState(m_dragSelectState);
    m_moveItemState = new QState(m_operationStateGroup);
    m_cloneItemState = new QState(m_operationStateGroup);
    m_moveHandleState = new QState(m_operationStateGroup);

    m_dragSelectState->addTransition(view(), SIGNAL(hoverItemEntered()), m_moveItemState);
    m_moveItemState->addTransition(view(), SIGNAL(hoverItemLeft()), m_dragSelectState);
    m_dragSelectState->addTransition(view(), SIGNAL(hoverHandleEntered()), m_moveHandleState);
    m_moveItemState->addTransition(view(), SIGNAL(hoverHandleLeft()), m_dragSelectState);
    m_moveItemState->addTransition(view(), SIGNAL(ctlKeyPressed()), m_cloneItemState);
    m_cloneItemState->addTransition(view(), SIGNAL(ctlKeyReleased()), m_moveItemState);
    m_dragSelectState->addTransition(view(), SIGNAL(hoverHandleEntered()), m_moveHandleState);
    m_moveHandleState->addTransition(view(), SIGNAL(hoverHandleLeft()), m_dragSelectState);

    m_stageStateGroup = new QState(m_topState);
    m_maybeState = new QState(m_stageStateGroup);
    m_stageStateGroup->setInitialState(m_maybeState);
    m_confirmedState = new QState(m_stageStateGroup);
    m_startedState = new QState(m_stageStateGroup);
    m_startedTransition = new QSignalTransition;

    m_maybeState->addTransition(view(), SIGNAL(leftMouseButtonPressed()), m_confirmedState);
    m_confirmedState->addTransition(view(), SIGNAL(mouseMoved()), m_startedState);
    m_startedTransition->setSenderObject(view());
    m_startedTransition->setSignal(SIGNAL(mouseMoved()));
    m_startedState->addTransition(m_startedTransition);
    m_startedState->addTransition(view(), SIGNAL(leftMouseButtonReleased()), m_maybeState);
    m_startedState->addTransition(view(), SIGNAL(escKeyReleased()), m_maybeState);

    connect(m_dragSelectState, &QState::entered,
            this, [this]() {
        qDebug() << "Drag select";
        view()->setCursor(Qt::ArrowCursor);
    });
    connect(m_dragSelectState, &QState::exited,
            this, [this]() {
    });
    connect(m_moveItemState, &QState::entered,
            this, [this]() {
        qDebug() << "Move item";
        view()->setCursor(Qt::SizeAllCursor);
        m_item = view()->objectUnderMouse();
        m_items = scene()->selectedObjects();
    });
    connect(m_cloneItemState, &QState::entered,
            this, [this]() {
        qDebug() << "Clone item";
        view()->setCursor(Qt::DragCopyCursor);
    });
    connect(m_moveHandleState, &QState::entered,
            this, [this]() {
        qDebug() << "Move handle";
        m_handle = view()->handleUnderMouse();
        view()->setCursor(m_handle->cursor());
    });
    connect(m_maybeState, &QState::entered,
            this, [this]() {
        qDebug() << "Maybe";
    });
    connect(m_confirmedState, &QState::entered,
            this, [this]() {
        qDebug() << "Confirmed";
       m_mousePressPosition = view()->mousePosition();
    });
    connect(m_startedState, &QState::entered,
            this, [this]() {
        qDebug() << "Started";
    });

    connect(m_confirmedState, &QState::entered,
            this, [this]() {
        if (m_dragSelectState->active()) {
            m_rubberBand->setGeometry(QRect(m_mousePressPosition, m_mousePressPosition));
            m_rubberBand->setVisible(true);
        }
    });
    connect(m_startedState, &QState::exited,
            this, [this]() {
        if (m_dragSelectState->active())
            m_rubberBand->setVisible(false);
    });
    connect(m_startedState, &QState::entered,
            this, &GraphicsSelectTool::handleMouseMove);
    connect(m_startedTransition, &QSignalTransition::triggered,
            this, &GraphicsSelectTool::handleMouseMove);
}

void GraphicsSelectTool::destroyStateMachine()
{
    delete m_stateMachine;
}

void GraphicsSelectTool::cancel()
{
}

void GraphicsSelectTool::setView(GraphicsView *other)
{
    if (view()) {
        destroyStateMachine();
        m_rubberBand->setParent(nullptr);
    }
    GraphicsTool::setView(other);
    if (other) {
        buildStateMachine();
        m_rubberBand->setParent(view());
    }
}

QString GraphicsSelectTool::toolGroup() const
{
    return "interactive-tools";
}

QAction *GraphicsSelectTool::action() const
{
    return  new QAction(QIcon::fromTheme("edit-select"),
                        "select", nullptr);
}
