#include "graphicsview.h"
#include "graphicsrectitem.h"

#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>

#include <QMouseEvent>
#include <QMessageBox>

// Tools:
//  Select (default)
//  ZoomBox
//  Drag
//  Insert
//  Edit
// Use graphical effect for item selection and move ?
// createPhantom() and createClone() to help the model command stuff

struct GraphicsView::MouseData {
    enum DragOperation {
        SelectOperation,
        MoveOperation,
        CloneOperation
    };

    //QMouseEvent *event;
    QPoint pos;
    Qt::MouseButton button;
    Qt::KeyboardModifiers modifiers;
    QGraphicsItem *itemUnderMouse;
    QList<QGraphicsItem *> phantomsItems;
    bool isDrag;
    DragOperation dragOperation;
    QGraphicsRectItem *dragAreaItem;
};


GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    m_mouseData = new MouseData;
    //m_mouseData->event = nullptr;
    m_mouseData->itemUnderMouse = nullptr;
    m_mouseData->isDrag = false;

    setRubberBandSelectionMode(Qt::IntersectsItemShape);
    setDragMode(QGraphicsView::RubberBandDrag);

    // The widget emits the QWidget::customContextMenuRequested() signal
    setContextMenuPolicy(Qt::CustomContextMenu);


}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{

}

// Select mode:
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

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (event->button() == Qt::LeftButton) {
        m_mouseData->pos = event->pos();
        m_mouseData->button = event->button();
        m_mouseData->modifiers = event->modifiers();
        m_mouseData->itemUnderMouse = itemAt(event->pos());
        m_mouseData->isDrag = false;
        event->accept();
    }
    else {
        event->ignore();
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_mouseData) {
        return;
    }

    // On the first time, decide which operation to perform, and initialise it
    if (!m_mouseData->isDrag) {
        m_mouseData->isDrag = true;
        if (!m_mouseData->itemUnderMouse) {
            // Drag select
            m_mouseData->dragOperation = GraphicsView::MouseData::SelectOperation;
            scene()->clearSelection();
            QGraphicsRectItem *item = new QGraphicsRectItem();
            item->setPos(0, 0);
            item->setOpacity(0.3);
            item->setBrush(Qt::darkCyan);
            item->setZValue(1E9);
            scene()->addItem(item);
            m_mouseData->dragAreaItem = item;
        }
        else {
            // Move or clone
            if (!m_mouseData->modifiers.testFlag(Qt::ControlModifier)) {
                // Move
                m_mouseData->dragOperation = GraphicsView::MouseData::MoveOperation;
                setCursor(Qt::DragMoveCursor);
            }
            else {
                // Clone
                m_mouseData->dragOperation = GraphicsView::MouseData::CloneOperation;
                setCursor(Qt::DragCopyCursor);
            }
            m_mouseData->phantomsItems.clear();
            foreach (QGraphicsItem *item, scene()->selectedItems()) {
                QGraphicsItem *phantom = createPhantomItem(item);
                scene()->addItem(phantom);
                m_mouseData->phantomsItems.append(phantom);
            }
        }
    }

    switch(m_mouseData->dragOperation) {
    case GraphicsView::MouseData::CloneOperation:
    case GraphicsView::MouseData::MoveOperation:
        foreach (QGraphicsItem *item, m_mouseData->phantomsItems) {
            item->setPos(mapToScene(event->pos())); // FIXME: use m_mouseData.event.pos()
        }
        break;
    case GraphicsView::MouseData::SelectOperation:
    {
        // FIXME: do this in drawForeground?
        QGraphicsRectItem *item = m_mouseData->dragAreaItem;
        QRect viewRect = QRect(m_mouseData->pos, event->pos()).normalized();
        QRectF rect = QRectF(item->mapFromScene(mapToScene(viewRect.topLeft())),
                             item->mapFromScene(mapToScene(viewRect.bottomRight())));
        item->setRect(rect.normalized());
        QPainterPath path;
        path.addPolygon(mapToScene(QRect(m_mouseData->pos,
                                         event->pos()).normalized()));
        scene()->setSelectionArea(path);
        break;
    }
    default:
        break;
    }

    event->accept();
    return;
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mouseData->isDrag) {
        switch(m_mouseData->dragOperation) {
        case GraphicsView::MouseData::CloneOperation:
            foreach (QGraphicsItem *item, m_mouseData->phantomsItems) {
                item->graphicsEffect()->setEnabled(false);
            }
            break;
        case GraphicsView::MouseData::MoveOperation:
            foreach (QGraphicsItem *item, m_mouseData->phantomsItems) {
                scene()->removeItem(item);
                m_mouseData->itemUnderMouse->setPos(item->pos());
                delete item;
            }
            break;
        case GraphicsView::MouseData::SelectOperation:
            scene()->removeItem(m_mouseData->dragAreaItem);
            delete m_mouseData->dragAreaItem;
            break;
        default:
            break;
        }
        setCursor(Qt::ArrowCursor);
    }
    else {
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
    }

    m_mouseData->itemUnderMouse = nullptr;
    m_mouseData->isDrag = false;
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    QGraphicsItem *item = scene()->itemAt(pos, transform());
    if (item == nullptr) {
        event->accept();
        return;
    }

    QMessageBox::information(this, "Tada!", "blam! blam! blam!",
                              QMessageBox::Close);
    event->accept();
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{

}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{

}

QGraphicsItem *GraphicsView::createPhantomItem(QGraphicsItem *item)
{
    GraphicsRectItem *rectItem = static_cast<GraphicsRectItem *>(item);
    rectItem = rectItem->clone();
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setEnabled(true);
    rectItem->setGraphicsEffect(effect);
    return rectItem;
}

QList<QGraphicsItem *> GraphicsView::createPhantomItems(QList<QGraphicsItem *> items)
{
    QList<QGraphicsItem *> rectItems;
    foreach (QGraphicsItem *item, items) {
        GraphicsRectItem *rectItem = static_cast<GraphicsRectItem *>(item);
        rectItems.append(rectItem->clone());
    }
    return rectItems;
}

