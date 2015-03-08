#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsObject;

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent);
    ~GraphicsScene();

    QList<GraphicsObject *> selectedObjects();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

};

#endif // GRAPHICSSCENE_H
