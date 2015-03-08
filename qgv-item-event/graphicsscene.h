#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent);
    ~GraphicsScene();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

};

#endif // GRAPHICSSCENE_H
