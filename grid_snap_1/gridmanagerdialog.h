#ifndef GRIDMANAGERDIALOG_H
#define GRIDMANAGERDIALOG_H

#include <QDialog>

class GridTableModel;
class QTableView;
class QMenu;
class QAction;

class GridManagerDialog : public QDialog
{
    Q_OBJECT

public:
    GridManagerDialog(QWidget *parent = 0);
    ~GridManagerDialog();

protected slots:
    void customMenuRequested(QPoint pos);
    void updateEditors();
    void add();
    void remove();
    void edit();
    void increment();
    void decrement();
    void enableAll();
    void disableAll();
    void import();
    void exportSelected();
    void exportAll();
    void apply();
    void reset();

private:
    GridTableModel *mModel;
    QTableView *mView;
    QMenu *mMenu;
    QAction *mIncrementAction;
    QAction *mDecrementAction;
    QAction *mRemoveAction;
    QAction *mEditAction;
};

#endif // GRIDMANAGERDIALOG_H
