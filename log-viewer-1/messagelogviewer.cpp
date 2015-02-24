#include "messagelogviewer.h"
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
    int i;
    j = 0;
}

MessageLogViewer::~MessageLogViewer()
{
    delete ui;
}
