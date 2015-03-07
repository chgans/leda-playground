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


struct GraphicsSelectTool::MouseData {
    enum DragOperation {
        NoDragOperation,
        SelectItemOperation,
        MoveItemOperation,
        MoveControlPointOperation,
        CloneItemOperation
    };

    MouseData(QWidget *parent, QPoint p, Qt::MouseButton b, Qt::KeyboardModifiers m,
              Qt::MouseEventFlags f, GraphicsObject *i, int c):
        pos(p), button(b), modifiers(m), flags(f), itemUnderMouse(i), ctlPoint(c),
        rubberBand(QRubberBand::Rectangle, parent), dragOperation(NoDragOperation)
    {}

    QPoint pos;
    Qt::MouseButton button;
    Qt::KeyboardModifiers modifiers;
    Qt::MouseEventFlags flags;
    GraphicsObject *itemUnderMouse;
    QList<QGraphicsItem *> items;
    QList<QGraphicsItem *> phantomItems;
    int ctlPoint;
    QRubberBand rubberBand;
    DragOperation dragOperation;
};

GraphicsSelectTool::GraphicsSelectTool(QObject *parent):
    GraphicsTool(parent), m_mouseData(nullptr)
{

}

GraphicsSelectTool::~GraphicsSelectTool()
{

}

void GraphicsSelectTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Q_ASSERT(m_mouseData == nullptr);
        QPointF scenePos = view()->mapToScene(event->pos());
        GraphicsObject *item = view()->objectAt(event->pos());
        int ctlPoint = -1;
        if (item != nullptr) {
            QPointF itemPos = item->mapFromScene(scenePos);
            ctlPoint = item->controlPointNear(itemPos);
        }
        m_mouseData = new MouseData(view(),
                                    event->pos(),
                                    event->button(),
                                    event->modifiers(),
                                    event->flags(),
                                    item,
                                    ctlPoint);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void GraphicsSelectTool::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_mouseData) {
        // No mouse press yet, Let's give a hint to the user by changing the cursor
        QGraphicsItem *gitem = view()->itemAt(event->pos());
        if (gitem != nullptr) {
            QPointF scenePos = view()->mapToScene(event->pos());
            GraphicsObject *item = view()->objectAt(event->pos());
            QPointF itemPos = item->mapFromScene(scenePos);
            int ctlPoint = item->controlPointNear(itemPos);
            if (ctlPoint != -1) {
                view()->setCursor(Qt::PointingHandCursor);
            }
            else {
                if (event->modifiers().testFlag(Qt::ControlModifier)) {
                    view()->setCursor(Qt::DragCopyCursor);
                }
                else {
                    view()->setCursor(Qt::SizeAllCursor);
                }
            }
        }
        else {
            view()->setCursor(Qt::ArrowCursor);
        }
        return;
    }

    // On the first time, decide which operation to perform, and initialise it
    if (m_mouseData->dragOperation == GraphicsSelectTool::MouseData::NoDragOperation) {
        if (!m_mouseData->itemUnderMouse) {
            // Drag select
            m_mouseData->dragOperation = GraphicsSelectTool::MouseData::SelectItemOperation;
            scene()->clearSelection();
            m_mouseData->rubberBand.setGeometry(QRect());
            m_mouseData->rubberBand.setVisible(true);
        }
        else {
            // Move item/control point or clone item
            if (m_mouseData->ctlPoint != -1) {
                m_mouseData->dragOperation = GraphicsSelectTool::MouseData::MoveControlPointOperation;
                view()->setCursor(Qt::PointingHandCursor);
                //ctlPoint->setActive(true);
            }
            else {
                if (!m_mouseData->modifiers.testFlag(Qt::ControlModifier)) {
                    // Move
                    m_mouseData->dragOperation = GraphicsSelectTool::MouseData::MoveItemOperation;
                    view()->setCursor(Qt::DragMoveCursor);
                }
                else {
                    // Clone
                    m_mouseData->dragOperation = GraphicsSelectTool::MouseData::CloneItemOperation;
                    view()->setCursor(Qt::DragCopyCursor);
                }
                if (!m_mouseData->itemUnderMouse->isSelected()) {
                    scene()->clearSelection();
                    m_mouseData->itemUnderMouse->setSelected(true);
                }

                m_mouseData->phantomItems = createPhantomItems(scene()->selectedItems());
            }
        }
    }

    switch(m_mouseData->dragOperation) {
    case GraphicsSelectTool::MouseData::CloneItemOperation:
    case GraphicsSelectTool::MouseData::MoveItemOperation:
    {
        Q_ASSERT(scene()->selectedItems().count() == m_mouseData->phantomItems.count());
        for (int i = 0; i < scene()->selectedItems().count(); i++) {
            QRectF sceneShift = QRectF(view()->mapToScene(m_mouseData->pos),
                                       view()->mapToScene(event->pos()));
            QPointF pos = scene()->selectedItems()[i]->pos();
            pos += QPointF(sceneShift.width(), sceneShift.height());
            m_mouseData->phantomItems[i]->setPos(pos);
        }
        break;
    }
    case GraphicsSelectTool::MouseData::MoveControlPointOperation:
    {
        GraphicsObject *obj = m_mouseData->itemUnderMouse;
        QPointF scenePos = view()->mapToScene(event->pos());
        QPointF itemPos = obj->mapFromScene(scenePos);
        obj->moveControlPoint(m_mouseData->ctlPoint, itemPos);
        break;
    }
    case GraphicsSelectTool::MouseData::SelectItemOperation:
    {
        QRect viewRect = QRect(m_mouseData->pos, event->pos()).normalized();
        m_mouseData->rubberBand.setGeometry(viewRect);
        QPainterPath path;
        path.addPolygon(view()->mapToScene(viewRect));
        scene()->setSelectionArea(path, Qt::IntersectsItemShape);
        break;
    }
    default:
        break;
    }

    event->accept();
    return;

}

void GraphicsSelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mouseData == nullptr) {
        return;
    }

    switch(m_mouseData->dragOperation) {

    case GraphicsSelectTool::MouseData::CloneItemOperation:
        foreach (QGraphicsItem *item, m_mouseData->phantomItems) {
            // Use document command stack here
            item->graphicsEffect()->setEnabled(false);
        }
        break;

    case GraphicsSelectTool::MouseData::MoveItemOperation:
        Q_ASSERT(scene()->selectedItems().count() == m_mouseData->phantomItems.count());
        for (int i = 0; i < m_mouseData->phantomItems.count(); i++) {
            // Use document command stack here
            QGraphicsItem *item = scene()->selectedItems().value(i);
            QGraphicsItem *phantomItem = m_mouseData->phantomItems.value(i);
            scene()->removeItem(phantomItem);
            item->setPos(phantomItem->pos());
            delete phantomItem;
        }
        break;

    case GraphicsSelectTool::MouseData::MoveControlPointOperation:
        break;

    case GraphicsSelectTool::MouseData::SelectItemOperation:
        break;

    case GraphicsSelectTool::MouseData::NoDragOperation:
        // Not a drag
        if (m_mouseData->itemUnderMouse) {
            // Not a drag, item under mouse
            QGraphicsItem *item = m_mouseData->itemUnderMouse;
            if (m_mouseData->modifiers.testFlag(Qt::ShiftModifier)) {
                // Not a drag, item under mouse, shift modifier
                item->setSelected(!item->isSelected());
                event->accept();
            }
            else {
                // Not a drag, item under mouse, no shift modifier
                bool wasSelected = item->isSelected();
                scene()->clearSelection();
                item->setSelected(!wasSelected);
                event->accept();
            }
        }
        else { // Not a drag, No item under mouse
            scene()->clearSelection();
            event->accept();
        }
        break;

    default:
        break;
    }

    view()->setCursor(Qt::ArrowCursor);
    delete m_mouseData;
    m_mouseData = nullptr;
}

void GraphicsSelectTool::mouseDoubleClickEvent(QMouseEvent *event)
{

}

QDialog *GraphicsSelectTool::optionDialog()
{
    return nullptr;
}

void GraphicsSelectTool::cancel()
{
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
