#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QList>

class QResizeEvent;
class QGraphicsItem;
class OverView;
class DetailView;
class InsightLensWidget;
class InsightHeadsUpWidget;
class InsightConnectivityWidget;
class InsightPickListWidget;

// TODO: rename to Pcb2dView
//  Add flags to enable tools (lens, ...)

class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = 0);

    void addMaskingItem(QGraphicsItem *item);
    void removeMaskingItem(QGraphicsItem *item);
    void setMaskingItems(QList<QGraphicsItem*> items);
    QList<QGraphicsItem*> maskingItems();
    void resetMaskingItems();

    // Needed for magnifier
    virtual void setScene(QGraphicsScene *scene);

signals:
    void viewportChanged();

public slots:

protected:
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool viewportEvent(QEvent *event);
    void drawForeground(QPainter * painter, const QRectF & rect);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    int mDesignInsightDelay;
    QTimer mDesignInsightTimer;
    QGraphicsItem *mDesignInsightItem;
    InsightConnectivityWidget *mConnectivity;
    /*
    QFrame *mDesignInsightFrame;
    OverView *mDesignInsightView;
    */

    QList<QGraphicsItem*> mMaskingItems;

    QPoint mLastMousePos;

    bool mMagnifier;
    QFrame *mMagnifierFrame;
    DetailView *mMagnifierView;

    InsightLensWidget *mLens;
    InsightHeadsUpWidget *mHeadsUp;

    InsightPickListWidget *mPickList;
    QGraphicsItem *mPickedItem;

protected slots:
    void showDesignInsight();
    void hideDesignInsight();

    void onItemSelectedFromPickList(QGraphicsItem* item);
};

#endif // MAINVIEW_H
