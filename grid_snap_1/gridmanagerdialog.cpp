#include "gridmanagerdialog.h"
#include "gridmanagerdialog_p.h"
#include "gridmanager.h"
#include "grideditordialog.h"

#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>

#include <QDebug>


// TODO: system grid

GridManagerDialog::GridManagerDialog(QWidget *parent)
    : QDialog(parent),
      mMenu(0)
{
    setWindowTitle("Grid Manager");
    setLayout(new QVBoxLayout);

    mView = new QTableView();
    mView->showGrid();
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout()->addWidget(mView);

    // Setup delegate editors
    CheckBoxDelegate *cbDelegate = new CheckBoxDelegate(this);
    mView->setItemDelegateForColumn(6, cbDelegate);
    mView->setItemDelegateForColumn(7, cbDelegate);
    //mView->openPersistentEditor(mModel->index(0, 6)); // FIXME
    //mView->openPersistentEditor(mModel->index(0, 7)); // FIXME
    ColorDelegate *colorDelegate = new ColorDelegate(this);
    mView->setItemDelegateForColumn(4, colorDelegate);
    mView->setItemDelegateForColumn(5, colorDelegate);
    //mView->openPersistentEditor(mModel->index(0, 4)); // FIXME
    //mView->openPersistentEditor(mModel->index(0, 5)); // FIXME

    // Double click fires up the grid editor dialog
    connect(mView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(edit()));

    //
    QDialogButtonBox *bbox = new QDialogButtonBox();
    bbox->setStandardButtons(QDialogButtonBox::Ok|
                             QDialogButtonBox::Cancel|
                             QDialogButtonBox::Apply|
                             QDialogButtonBox::Reset);
    connect(bbox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            SIGNAL(applyRequested()));
    connect(bbox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            SIGNAL(resetRequested()));
    connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            SLOT(accept()));
    connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            SLOT(reject()));

    layout()->addWidget(bbox);

    // Context menu setup
    mView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));

}

GridManagerDialog::~GridManagerDialog()
{

}

void GridManagerDialog::setModel(GridTableModel *model)
{
    if (mModel != nullptr)
        mModel->disconnect(this);

    mModel = model;

    if (mModel == nullptr)
        return;

    mView->setModel(mModel);

    // Setup header sizes
    QHeaderView *header = mView->horizontalHeader();
    header->setStretchLastSection(false);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::Stretch);
    header->setSectionResizeMode(7, QHeaderView::Fixed);
    header->resize(7, header->sectionSize(6));

    // Setup dialog size
    resize(QSize(header->size().width()+100,
           25*QLabel("A").sizeHint().height()));


    mView->sortByColumn(0);

    connect(mModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(updateEditors()));
    connect(mModel, SIGNAL(modelReset()),
            this, SLOT(updateEditors()));

    //
    updateEditors();
}

void GridManagerDialog::customMenuRequested(QPoint pos)
{
    QModelIndex index = mView->indexAt(pos);
    if (!mMenu) {
        mMenu = new QMenu(this);
        QAction *action;
        GridManager *mng = GridManager::instance();
        foreach (QString id, mng->gridTypeList()) {
            action = mMenu->addAction(QString("Add %1 grid...").arg(mng->gridTypeLabel(id)));
            action->setData(QVariant::fromValue(id));
            connect(action, SIGNAL(triggered()),
                    this, SLOT(add()));
        }
        mMenu->addSeparator();
        mIncrementAction = mMenu->addAction("Increment priority");
        connect(mIncrementAction, SIGNAL(triggered()),
                this, SLOT(increment()));
        mDecrementAction = mMenu->addAction("Decrement priority");
        connect(mDecrementAction, SIGNAL(triggered()),
                this, SLOT(decrement()));
        mMenu->addSeparator();
        action = mMenu->addAction("Enable all custom grids");
        connect(action, SIGNAL(triggered()),
                this, SLOT(enableAll()));
        action = mMenu->addAction("Disable all custom grids");
        connect(action, SIGNAL(triggered()),
                this, SLOT(disableAll()));
        mMenu->addSeparator();
        mEditAction = mMenu->addAction("Properties");
        connect(mEditAction, SIGNAL(triggered()),
                this, SLOT(edit()));
        mMenu->addSeparator();
        mRemoveAction = mMenu->addAction("Delete");
        connect(mRemoveAction, SIGNAL(triggered()),
                this, SLOT(remove()));
        mMenu->addSeparator();
        action = mMenu->addAction("Import grids...");
        action->setEnabled(false);
        action = mMenu->addAction("Export selected grids...");
        action->setEnabled(false);
        action = mMenu->addAction("Export all grids...");
        action->setEnabled(false);
    }

    bool enabled;
    enabled = index.isValid() &&
            index.row() != 0 &&
            index.row() != mModel->rowCount() - 1;
    mIncrementAction->setEnabled(enabled);
    enabled = index.isValid() &&
            index.row() != mModel->rowCount() - 1 &&
            (mModel->rowCount() > 1 && index.row() != mModel->rowCount() - 2);
    mDecrementAction->setEnabled(enabled);
    enabled = index.isValid() &&
            index.row() != mModel->rowCount() - 1;
    mRemoveAction->setEnabled(enabled);
    enabled = index.isValid();
    mEditAction->setEnabled(enabled);

    mMenu->popup(mView->viewport()->mapToGlobal(pos));
}

void GridManagerDialog::updateEditors()
{
    mView->sortByColumn(0);

    for (int row = 0; row < mModel->rowCount(); ++row)
        for (int col = 4; col <= 7; ++col) {
            mView->openPersistentEditor(mModel->index(row, col));
        }
}

void GridManagerDialog::add()
{
    GridManager *mng = GridManager::instance();
    QAction *action = static_cast<QAction*>(sender());
    QString typeId = action->data().toString();
    Q_ASSERT(action);
    IGrid *grid = mng->createGrid(typeId);
    Q_ASSERT(grid);
    IGridEditor *editor = mng->createGridEditor(typeId);
    Q_ASSERT(editor);

    if (GridEditorDialog::editGrid(grid, editor, this) != 0) {
        mModel->addGrid(grid);
    }
}

void GridManagerDialog::remove()
{
    qDebug() << __FUNCTION__;
    QModelIndexList list = mView->selectionModel()->selectedRows();
    if (list.isEmpty())
        return;
    foreach (QModelIndex index, list) {
        mModel->removeGrid(mModel->grid(index));
    }
}

void GridManagerDialog::edit()
{
    QModelIndexList list = mView->selectionModel()->selectedRows();
    if (list.isEmpty())
        return;

    int row = list[0].row();

    // Select only the first one
    mView->selectRow(row);
    IGrid *grid = mModel->grid(list[0])->clone();

    IGridEditor *editor = GridManager::instance()->createGridEditor(grid->typeId());
    Q_ASSERT(grid);
    Q_ASSERT(editor);

    if (GridEditorDialog::editGrid(grid, editor, this) != 0) {
        mModel->setGrid(row, grid);
    }
}

void GridManagerDialog::increment()
{
    foreach (QModelIndex index, mView->selectionModel()->selectedRows(0)) {
        int prio = mModel->data(index, Qt::DisplayRole).toInt();
        mModel->setData(index, prio + 1, Qt::DisplayRole);
    }
}

void GridManagerDialog::decrement()
{
    qDebug() << __FUNCTION__;
}

void GridManagerDialog::enableAll()
{
    qDebug() << __FUNCTION__;
}

void GridManagerDialog::disableAll()
{
    qDebug() << __FUNCTION__;
}

void GridManagerDialog::import()
{
    qDebug() << __FUNCTION__;
}

void GridManagerDialog::exportSelected()
{
    qDebug() << __FUNCTION__;
}

void GridManagerDialog::exportAll()
{
    qDebug() << __FUNCTION__;
}
