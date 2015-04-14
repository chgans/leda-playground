#include "insightdockwidget.h"

#include "objectpreview.h"
#include "detailview.h"
#include "overview.h"

#include <QSplitter>

InsightDockWidget::InsightDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    m_objectPreview = new ObjectPreview();
    m_detailView = new DetailView();
    m_overview = new OverView();
    splitter->addWidget(m_objectPreview);
    splitter->addWidget(m_detailView);
    splitter->addWidget(m_overview);
}
