#ifndef GRIDEDITORDIALOG_H
#define GRIDEDITORDIALOG_H

#include <QDialog>

class IGrid;
class IGridEditor;

class GridEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GridEditorDialog(IGrid *grid,
                              IGridEditor *editor,
                              QWidget *parent = 0);

    // return grid if modified and accepted, 0 otherwise
    static IGrid *editGrid(IGrid *grid,
                           IGridEditor *editor,
                           QWidget *parent = 0);

private:
    IGrid *mGrid;
    IGridEditor *mEditor;

    // QDialog interface
public slots:
    void done(int);
    void apply();
    void reset();
};

#endif // GRIDEDITORDIALOG_H
