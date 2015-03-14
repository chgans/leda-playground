#include "grideditordialog.h"
#include "igrid.h"
#include "igrideditor.h"
#include "igridfactory.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QWidget>

GridEditorDialog::GridEditorDialog(IGrid *grid,
                                   IGridEditor *editor,
                                   QWidget *parent) :
    QDialog(parent),
    mGrid(grid),
    mEditor(editor)
{
    setLayout(new QVBoxLayout);
    setWindowTitle(mEditor->title());

    mEditor->loadFromGrid(mGrid);
    layout()->addWidget(mEditor);

    QDialogButtonBox *bbox = new QDialogButtonBox();
    bbox->setStandardButtons(QDialogButtonBox::Ok|
                             QDialogButtonBox::Cancel/*|
                             QDialogButtonBox::Apply|
                             QDialogButtonBox::Reset*/);
    layout()->addWidget(bbox);
    connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            SLOT(accept()));
    connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            SLOT(reject()));
    /*
    connect(bbox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            SLOT(apply()));
    connect(bbox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            SLOT(reset()));
            */
}

IGrid *GridEditorDialog::editGrid(IGrid *grid, IGridEditor *editor, QWidget *parent)
{
    if (GridEditorDialog(grid, editor, parent).exec() == QDialog::Accepted)
        return grid;
    else
        return 0;
}

void GridEditorDialog::done(int result)
{
    if (result == QDialog::Accepted) {
        mEditor->saveToGrid(mGrid);
    }
    QDialog::done(result);
}

void GridEditorDialog::apply()
{

}

void GridEditorDialog::reset()
{
    mEditor->loadFromGrid(mGrid);
}

