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

    void setModel(GridTableModel *model);

signals:
    void applyRequested();
    void resetRequested();

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

private:
    GridTableModel *mModel = nullptr;
    QTableView *mView = nullptr;
    QMenu *mMenu = nullptr;
    QAction *mIncrementAction = nullptr;
    QAction *mDecrementAction = nullptr;
    QAction *mRemoveAction =nullptr;
    QAction *mEditAction = nullptr;
};

#endif // GRIDMANAGERDIALOG_H
