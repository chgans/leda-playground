#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class PackageItemForm;
class DocumentModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void onItemClicked(const QModelIndex &item);

private:
    Ui::MainWindow *ui;
    DocumentModel *m_model;
    PackageItemForm *m_form;
};

#endif // MAINWINDOW_H
