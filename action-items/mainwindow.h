#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QDockWidget;
class MenuItem;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addDocks();
    void populateMenu(QMenu *topMenu, MenuItem *topItem);
    void populateTreeView(QTreeWidgetItem *topWidgetItem, MenuItem *topMenuItem);

protected slots:
    void showHideLeftDockArea(bool show);
    void showHideRightDockArea(bool show);
    void showHideOutputPane(int nb);

    void setDockWidgetAreaVisible(Qt::DockWidgetArea area, bool visible);

private:
    Ui::MainWindow *ui;
    QList<QDockWidget*> m_outputPanes;
    QList<QDockWidget*> m_hiddenLeftDockWidgets;
    QList<QDockWidget*> m_hiddenRightDockWidgets;
};

#include <QUndoCommand>


#endif // MAINWINDOW_H
