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

    // The size of the cells in the grid.
    QSize m_cellSize;

};

#endif // GRAPHICSSCENE_H
