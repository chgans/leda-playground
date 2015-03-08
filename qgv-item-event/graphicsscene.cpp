#include "graphicsscene.h"

#include <QPainter>

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(sceneRect(), Qt::black);
}

