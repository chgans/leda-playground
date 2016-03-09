#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"
#include <QGraphicsScene>

class TrackElement;

enum CircularMotionDirection
{
    ClockWiseDirection = 0,
    CounterClockWiseDirection,
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void WalkAround(const TrackElement *subject, const TrackElement *obstacle,
                    qreal clearance);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    GraphicsView *m_view;
};

#endif // MAINWINDOW_H
