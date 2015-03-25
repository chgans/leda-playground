#ifndef INSIGHTHEADSUPWIDGET_H
#define INSIGHTHEADSUPWIDGET_H

#include <QFrame>

class QGridLayout;
class QLabel;
class QGraphicsView;

class InsightHeadsUpWidget : public QFrame
{
    Q_OBJECT
public:
    explicit InsightHeadsUpWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void updateCoordinates();
private:
    QGraphicsView *mView;
    QGridLayout *mCoordLayout;
    QLabel *mX;
    QLabel *mY;
    QLabel *mDX;
    QLabel *mDY;
};

#endif // INSIGHTHEADSUPWIDGET_H
