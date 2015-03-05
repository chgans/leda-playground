#include "graphicsscene.h"

#include <QPainter>

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{

    m_cellSize.setHeight(10);
    m_cellSize.setWidth(10);
    //m_dragged = 0;
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, Qt::darkGray);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, QBrush(gradient));

    painter->fillRect(sceneRect(), Qt::black);
/*
    QRectF rect2 = rect.intersected(sceneRect());
    qreal left = int(rect2.left()) - (int(rect2.left()) % m_cellSize.width());
    qreal top = int(rect2.top()) - (int(rect2.top()) % m_cellSize.height());
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = left; x < rect2.right(); x += m_cellSize.width())
        lines.append(QLineF(x, rect2.top(), x, rect2.bottom()));
    for (qreal y = top; y < rect2.bottom(); y += m_cellSize.height())
        lines.append(QLineF(rect2.left(), y, rect2.right(), y));
    painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter->drawLines(lines.data(), lines.size());
    */
}

