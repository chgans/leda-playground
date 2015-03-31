#ifndef INSIGHTHEADSUPWIDGET_H
#define INSIGHTHEADSUPWIDGET_H

#include <QFrame>

class QGridLayout;
class QLabel;
class QGraphicsView;

// TODO:
// - enableHeadsUpTracking
// - resetHeadsUpDeltaOrigin
// - enableHeadsUpDeltaOrigin
class InsightHeadsUpWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(bool mouseTracking READ mouseTracking WRITE setMouseTracking NOTIFY mouseTrackingChanged)
    Q_PROPERTY(bool deltaOrigin READ deltaOrigin WRITE setDeltaOrigin NOTIFY deltaOriginChanged)

public:
    explicit InsightHeadsUpWidget(QWidget *parent = 0);

    bool mouseTracking() const;
    bool deltaOrigin() const;

signals:
    void mouseTrackingChanged(bool arg);
    void deltaOriginChanged(bool arg);

public slots:
    void setMouseTracking(bool arg);
    void setDeltaOrigin(bool arg);
    void resetDeltaOrigin();

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
    bool m_mouseTracking;
    bool m_deltaOrigin;
};

#endif // INSIGHTHEADSUPWIDGET_H
