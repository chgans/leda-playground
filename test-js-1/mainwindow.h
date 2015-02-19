#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJSEngine>
#include <QtMath>

namespace Ui {
class MainWindow;
}

class JSMath: public QObject
{
    Q_OBJECT

public:
    explicit JSMath(QObject *parent = 0):
        QObject(parent)
    {}

    ~JSMath()
    {}

public slots:
    qreal acos(qreal r)
    { return qAcos(r); }
    qreal cos(qreal r)
    { return qCos(r); }
    qreal asin(qreal r)
    { return qAsin(r); }
    qreal sin(qreal r)
    { return qSin(r); }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Q_SLOT void execJS();

private:
    Ui::MainWindow *ui;

    QJSEngine *mJSEngine;
};

#endif // MAINWINDOW_H
