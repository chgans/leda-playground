#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *mScene;
    QGraphicsItem *mPort;
    QGraphicsLineItem *mPortPin;
    QGraphicsSimpleTextItem *mPortName;
    QGraphicsSimpleTextItem *mPortId;
};

#endif // MAINWINDOW_H
