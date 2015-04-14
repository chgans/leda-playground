#ifndef INSIGHTDOCKWIDGET_H
#define INSIGHTDOCKWIDGET_H

#include <QDockWidget>

class ObjectPreview;
class DetailView;
class OverView;
class MainView;

class InsightDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit InsightDockWidget(QWidget *parent = 0);

    void attachView(MainView *view);
    void detachView();
    MainView *attachedView() const;

signals:

public slots:

private:
    MainView *m_mainView;
    ObjectPreview *m_objectPreview;
    DetailView *m_detailView;
    OverView *m_overview;
};

#endif // INSIGHTDOCKWIDGET_H
