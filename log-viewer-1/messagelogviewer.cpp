#include "messagelogviewer.h"
#include "loggingmodel.h"
#include "ui_messagelogviewer.h"

// FreeDesktop Icon theme:
// - For severity:
//  - dialog-information
//  - dialog-warning
//  - dialog-error
// - For Categories
//  - applications-utilities
//  - applications-system
//  - applications-science

MessageLogViewer::MessageLogViewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MessageLogViewer)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MessageLogViewer::~MessageLogViewer()
{
    delete ui;
}

LoggingModel *MessageLogViewer::model()
{
    return m_model;
}

void MessageLogViewer::setModel(LoggingModel *model)
{
    m_model = model;
    ui->tableView->setModel(model);
}
