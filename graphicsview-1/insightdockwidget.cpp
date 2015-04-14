#include "insightdockwidget.h"

#include "mainview.h"
#include "objectpreview.h"
#include "detailview.h"
#include "overview.h"

#include <QSplitter>

InsightDockWidget::InsightDockWidget(QWidget *parent) :
    QDockWidget(parent),
    m_mainView(nullptr)
{
    m_objectPreview = new ObjectPreview();
    m_detailView = new DetailView();
    m_overview = new OverView();
    m_overview->setMaximumWidth(300); // FIXME

    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(m_objectPreview);
    splitter->addWidget(m_detailView);
    splitter->addWidget(m_overview);
    setWidget(splitter);
}

void InsightDockWidget::attachView(MainView *view)
{
    if (m_mainView)
        detachView();
    m_mainView = view;
    if (m_mainView) {
        m_detailView->setObservedView(view);
        m_overview->setObservedView(view);
    }
}

void InsightDockWidget::detachView()
{
    m_detailView->setObservedView(nullptr);
    m_overview->setObservedView(nullptr);
}

MainView *InsightDockWidget::attachedView() const
{
    return m_mainView;
}
