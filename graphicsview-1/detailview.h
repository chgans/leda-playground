#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#include <QGraphicsView>

// Close-up view or lense view
// Show a close up view of the area under the mouse cursor
// Non-interactive
class DetailView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DetailView(QWidget *parent = 0);
    void setObservedView(QGraphicsView *view);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    QGraphicsView *mObservedView;
};

#endif // DETAILVIEW_H
