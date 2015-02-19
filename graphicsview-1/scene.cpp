#include "scene.h"

#include <QCursor>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <qmath.h>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    init();
}

Scene::Scene(const QRectF &sceneRect, QObject *parent):
    QGraphicsScene(sceneRect, parent)
{
    init();
}

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent):
    QGraphicsScene(x, y, width, height, parent)
{
    init();
}

void Scene::init()
{
    mCellSize.setHeight(25);
    mCellSize.setWidth(25);
    mDragged = 0;
}

QPointF Scene::snapScenePos(QPointF pos)
{
    int x = qRound(pos.x() / mCellSize.width()) * mCellSize.width();
    int y = qRound(pos.y() / mCellSize.height()) * mCellSize.height();
    return QPointF(x, y);
}

// Efficiently draws a grid in the background.
// For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, Qt::darkGray);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, QBrush(gradient));

    painter->fillRect(sceneRect(), Qt::black);

    QRectF rect2 = rect.intersected(sceneRect());
    qreal left = int(rect2.left()) - (int(rect2.left()) % mCellSize.width());
    qreal top = int(rect2.top()) - (int(rect2.top()) % mCellSize.height());
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = left; x < rect2.right(); x += mCellSize.width())
        lines.append(QLineF(x, rect2.top(), x, rect2.bottom()));
    for (qreal y = top; y < rect2.bottom(); y += mCellSize.height())
        lines.append(QLineF(rect2.left(), y, rect2.right(), y));
    painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter->drawLines(lines.data(), lines.size());
}


// TODO: do this in the view
// TODO: the cross has to be the same size independently of the zoom factor

void Scene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)

    // Draw big cross
    if (mDragged) {
        painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
        painter->drawLine(mDragged->scenePos().x() - mCellSize.width()/2.0,
                          mDragged->scenePos().y(),
                          mDragged->scenePos().x() + mCellSize.width()/2.0,
                          mDragged->scenePos().y());
        painter->drawLine(mDragged->scenePos().x(),
                          mDragged->scenePos().y() - mCellSize.height()/2.0,
                          mDragged->scenePos().x(),
                          mDragged->scenePos().y() + mCellSize.height()/2.0);
    }

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*
    mDragged = qgraphicsitem_cast<QGraphicsItem*>(itemAt(mouseEvent->scenePos(), QTransform()));
    if (mDragged) {
        QGraphicsView *view = views().first();
        QPoint screenPos = view->mapToGlobal(view->mapFromScene(mDragged->pos()));
        QCursor::setPos(screenPos);
    }
    */
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*
    if (mDragged) {
        mDragged->setPos(snapScenePos(mouseEvent->scenePos()));
    } else */
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*
    if (mDragged) {
        mDragged->setPos(snapScenePos(mouseEvent->scenePos()));
        mDragged = 0;
    } else*/
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

