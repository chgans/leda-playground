#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoggingModel;
class LoggingWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    LoggingModel *loggingModel();

private:
    Ui::MainWindow *ui;
    LoggingModel *m_logModel;
    LoggingWidget *m_logViewer;
};

#endif // MAINWINDOW_H
