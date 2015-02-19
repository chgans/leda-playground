#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class DocumentModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setModel(DocumentModel *model);
private:
    Ui::MainWindow *ui;
    DocumentModel *m_model;
};

#endif // MAINWINDOW_H
