#ifndef INSIGHTDOCKWIDGET_H
#define INSIGHTDOCKWIDGET_H

#include <QDockWidget>

class ObjectPreview;
class DetailView;
class OverView;

class InsightDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit InsightDockWidget(QWidget *parent = 0);

signals:

public slots:

private:
    ObjectPreview *m_objectPreview;
    DetailView *m_detailView;
    OverView *m_overview;
};

#endif // INSIGHTDOCKWIDGET_H
