#include "loggingwidget.h"
#include "loggingmodel.h"
#include "ui_loggingwidget.h"
#include "loggingsortfilterproxymodel.h"

#include <QMenu>

LoggingWidget::LoggingWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LoggingWidget),
    m_model(nullptr),
    m_filterModel(new LoggingSortFilterProxyModel(this)),
    m_categoriesMenu(new QMenu(this))
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    //ui->tableView->resizeColumnsToContents();

    ui->configToolButton->setMenu(m_categoriesMenu);
    connect(m_categoriesMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(filterCategoriesTriggered(QAction*)));

    connect(ui->infoToolButton, &QToolButton::toggled,
            m_filterModel, &LoggingSortFilterProxyModel::setFilterIncludesDebugMessages);
    connect(ui->warningToolButton, &QToolButton::toggled,
            m_filterModel, &LoggingSortFilterProxyModel::setFilterIncludesWarningMessages);
    connect(ui->errorToolButton, &QToolButton::toggled,
            m_filterModel, &LoggingSortFilterProxyModel::setFilterIncludesCriticalMessages);
    connect(ui->errorToolButton, &QToolButton::toggled,
            m_filterModel, &LoggingSortFilterProxyModel::setFilterIncludesFatalMessages);
    ui->infoToolButton->setChecked(true);
    ui->warningToolButton->setChecked(true);
    ui->errorToolButton->setChecked(true);

    connect(ui->previousToolButton, &QToolButton::clicked,
            this, &LoggingWidget::goToPrevious);
    connect(ui->nextToolButton, &QToolButton::clicked,
            this, &LoggingWidget::goToNext);

    connect(m_filterModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(updateNavigationState()));
    connect(m_filterModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(updateNavigationState()));
    connect(m_filterModel, SIGNAL(modelReset()),
            this, SLOT(updateNavigationState()));
    updateNavigationState();

    m_filterModel->setFilterKeyColumn(-1); // filter on all columns
    connect(ui->filterLineEdit, SIGNAL(textChanged(QString)),
            m_filterModel, SLOT(setFilterFixedString(QString)));
}

LoggingWidget::~LoggingWidget()
{
    delete ui;
}

LoggingModel *LoggingWidget::model()
{
    return m_model;
}

void LoggingWidget::setModel(LoggingModel *model)
{
    if (m_model) {
        m_model->disconnect(this);
        ui->clearToolButton->disconnect(m_model);
        ui->tableView->selectionModel()->disconnect(this);
    }

    m_model = model;
    m_filterModel->setSourceModel(model);
    ui->tableView->setModel(m_filterModel);

    if (m_model) {
#if 0
        connect(m_model, &LoggingModel::rowsInserted,
                this, [this]() {
            this->ui->tableView->scrollToBottom();
        });
#endif
        connect(ui->clearToolButton, &QToolButton::clicked,
                this, [this](bool checked) {
            Q_UNUSED(checked);
            this->m_model->clearMessages();
        });
        connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(updateNavigationState()));
        connect(m_model, SIGNAL(categoryListChanged(QList<const char*>)),
                this, SLOT(updateCategoryList(QList<const char*>)));
    }
}

bool LoggingWidget::canNavigate() const
{
    return m_filterModel->rowCount() != 0;
}

bool LoggingWidget::canGoToNext() const
{
    if (!canNavigate())
        return false;

    if (!ui->tableView->currentIndex().isValid())
        return true;

    return ui->tableView->currentIndex().row() != m_filterModel->rowCount() - 1;
}

bool LoggingWidget::canGoToPrevious() const
{
    if (!canNavigate())
        return false;

    if (!ui->tableView->currentIndex().isValid())
        return true;

    return ui->tableView->currentIndex().row() != 0;
}

void LoggingWidget::goToNext()
{
    if (!canNavigate())
        return;

    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) {
        ui->tableView->setCurrentIndex(m_filterModel->index(0, 0));
        return;
    }

    QModelIndex next = m_filterModel->index(current.row()+1, 0);
    ui->tableView->setCurrentIndex(next);

}

void LoggingWidget::goToPrevious()
{
    if (!canNavigate())
        return;

    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) {
        ui->tableView->setCurrentIndex(m_filterModel->index(0, 0));
        return;
    }

    QModelIndex next = m_filterModel->index(current.row()-1, 0);
    ui->tableView->setCurrentIndex(next);
}

void LoggingWidget::updateCategoryList(const QList<const char *> categories)
{
    m_categoriesMenu->clear();
    foreach (const char *name, categories) {
        QAction *action = new QAction(m_categoriesMenu);
        action->setCheckable(true);
        action->setText(QString(QByteArray(name)));
        action->setData((quint64)name);
        action->setChecked(m_filterModel->filterIncludesCategoryName(name));
        m_categoriesMenu->addAction(action);
    }
}

void LoggingWidget::filterCategoriesTriggered(QAction *action)
{
    const char *category = (const char *)action->data().toInt(); // FIXME!
    setCategoryVisibility(category, action->isChecked());
}

void LoggingWidget::setCategoryVisibility(const char *categoryName, bool visible)
{
    m_filterModel->setFilterIncludesCategoryName(categoryName, visible);
}

void LoggingWidget::updateNavigationState()
{
    ui->nextToolButton->setEnabled(canGoToNext());
    ui->previousToolButton->setEnabled(canGoToPrevious());
}
