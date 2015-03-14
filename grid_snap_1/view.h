#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class GridManager;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);

    void initialise();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);

    void paintEvent(QPaintEvent *event);
    void setCursorPosition(const QPointF &pos);

signals:

public slots:

private:
    GridManager *mGridManager;
    QPointF mCursorPosition;

    void drawGrids(QPainter *painter, const QRectF &rect);
    void drawCursor(QPainter *painter, const QRectF &rect);

    QGraphicsRectItem *mBoard;

    enum AngleConstraint {
        AnyAngle = 0,
        RightAngle,
        FDiagAngle,
        BDiagAngle,
    };
    qreal mStartAngle;
    qreal mEndAngle;
    AngleConstraint mAngleConstraint;
    void cycleAngleConstraint();

    enum CornerStyle {
        PlainCorner = 0,
        ArcCorner,
        MitterCorner
    };
    CornerStyle mCornerStyle;
    void cycleCornerStyle();

    bool mSwapStartEndAngles;
    void toggleSwapStartEndAngles();

    QGraphicsPathItem *mPathItem;
    void startPathItem(QPointF point);
    void updatePathItem();
    void finishPathItem();

    QList<QPainterPath> mSubPath;
    void startSubPath(QPointF point);
    void updateSubPath(QPointF point);
    void finishSubPath(QPointF point);

};


#endif // VIEW_H
