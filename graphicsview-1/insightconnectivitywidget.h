#ifndef INSIGHTCONNECTIVITYWIDGET_H
#define INSIGHTCONNECTIVITYWIDGET_H

#include <QFrame>

class QGraphicsView;
class OverView;

class InsightConnectivityWidget : public QFrame
{
    Q_OBJECT
public:
    explicit InsightConnectivityWidget(QWidget *parent = 0);

    // TODO:
    // setProject(), setHighlightedObject(Idoc doc, IObj obj)
    void setBuddyView(QGraphicsView *view);

signals:

public slots:

private:
    OverView *m_view;
};

#endif // INSIGHTCONNECTIVITYWIDGET_H
