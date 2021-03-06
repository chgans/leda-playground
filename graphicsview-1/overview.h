#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QGraphicsView>

class OverView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit OverView(QWidget *parent = 0);

    virtual void setScene(QGraphicsScene *scene);
    void setObservedView(QGraphicsView *view);

signals:

protected slots:
    void getObservedRect();

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool eventFilter(QObject *obj, QEvent *ev);

    void forceRedraw();

private:
    enum Move {
        NoMove = 0,
        MoveRect,
        MoveTopLeft,
        MoveTopRight,
        MoveBottomRight,
        MoveBottomLeft
    };
    QGraphicsView *m_observedView;
    QRectF m_observedRect;
    Move m_move;
    QPointF m_lastPos;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // OVERVIEW_H
