#include "mainwindow.h"
#include <QApplication>

#include "tests.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    tst_StraightPathRouter_positiveDirection();
    tst_StraightPathRouter_negativeDirection();
    tst_DiagonalPathRouter_positiveDirection();
    tst_DiagonalPathRouter_negativeDirection();

    return a.exec();
}
