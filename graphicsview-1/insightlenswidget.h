#ifndef INSIGHTLENSWIDGET_H
#define INSIGHTLENSWIDGET_H

#include <QFrame>

class QGraphicsView;

class InsightLensWidget : public QFrame
{
    Q_OBJECT

public:
    enum LensShape {
        SquareLens = 0,
        RoundLens = 1
    };

    explicit InsightLensWidget(QWidget *parent = 0);

    // TODO: Q_PROPERTY

    LensShape lensShape() const;
    QSize lensSize() const;
    int lensZoomLevel() const;
    bool mouseTracking() const;
    bool isLensEnabled() const;

    void moveLensToMousePosition();
    void moveLensContentToMousePosition();

    void setBuddyView(QGraphicsView *view);
    QGraphicsView *buddyView() const;

signals:

public slots:
    void setLensShape(LensShape shape);
    void toggleLensShape();
    void setLensSize(const QSize &size);
    void setLensZoomLevel(int percent);
    void setMouseTracking(bool enable);
    bool toggleMouseTracking();
    void setLensEnabled(bool enable);
    bool toggleLensEnabled();

protected:
    //bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);
    bool handleWheelEvent(QWheelEvent *event);

private:
    LensShape mShape;
    QSize mSize;
    bool mMouseTracking;
    bool mEnabled;
    int mZoomLevel;
    QGraphicsView *mView;
    QGraphicsView *mBuddyView;

    void fixupPaintingArtifacts();
};

#endif // INSIGHTLENSWIDGET_H
