#include "tests.h"
#include "ipathrouter.h"

#include <QLineF>
#include <QList>
#include <QPainterPath>

#include <QDebug>

static QList<QLineF> inputs = QList<QLineF>()
        << QLineF(0, 0,  10,  04)
        << QLineF(0, 0,  04,  10)
        << QLineF(0, 0, -04,  10)
        << QLineF(0, 0, -10,  04)
        << QLineF(0, 0, -10, -04)
        << QLineF(0, 0, -04, -10)
        << QLineF(0, 0,  04, -10)
        << QLineF(0, 0,  10, -04);

void tst_StraightPathRouter_positiveDirection()
{
    QList<QPainterPath> outputs;
    QPainterPath path0;
    path0.moveTo(  0,   0);
    path0.lineTo( 10,   0);
    path0.lineTo( 10,   4);
    outputs.append(path0);
    QPainterPath path1;
    path1.moveTo(  0,   0);
    path1.lineTo(  4,   0);
    path1.lineTo(  4,  10);
    outputs.append(path1);
    QPainterPath path2;
    path2.moveTo(  0,   0);
    path2.lineTo(  0,  10);
    path2.lineTo(- 4,  10);
    outputs.append(path2);
    QPainterPath path3;
    path3.moveTo(  0,   0);
    path3.lineTo(  0,   4);
    path3.lineTo(-10,   4);
    outputs.append(path3);
    QPainterPath path4;
    path4.moveTo(  0,   0);
    path4.lineTo(-10,   0);
    path4.lineTo(-10, - 4);
    outputs.append(path4);
    QPainterPath path5;
    path5.moveTo(  0,   0);
    path5.lineTo(- 4,   0);
    path5.lineTo(- 4, -10);
    outputs.append(path5);
    QPainterPath path6;
    path6.moveTo(  0,   0);
    path6.lineTo(  0, -10);
    path6.lineTo(  4, -10);
    outputs.append(path6);
    QPainterPath path7;
    path7.moveTo(  0,   0);
    path7.lineTo(  0, - 4);
    path7.lineTo( 10, - 4);
    outputs.append(path7);

    for (int i = 0; i < inputs.count(); i++)
    {
        QLineF input = inputs[i];
        QPainterPath expected = outputs[i];
        StraightPathRouter router;
        router.start(input.p1());
        QPainterPath actual = router.route(input.p2());
        if (actual != expected)
        {
            qDebug() << "SP FAIL" << i;
            qDebug() << "SP  input:" << input;
            qDebug() << "SP  actual:" << actual;
            qDebug() << "SP  expected:" << expected;
        }
        else
        {
            qDebug() << "SP PASS" << i;
        }
    }
}

void tst_StraightPathRouter_negativeDirection()
{
    QList<QPainterPath> outputs;
    QPainterPath path0;
    path0.moveTo(  0,   0);
    path0.lineTo(  0,   4);
    path0.lineTo( 10,   4);
    outputs.append(path0);
    QPainterPath path1;
    path1.moveTo(  0,   0);
    path1.lineTo(  0,  10);
    path1.lineTo(  4,  10);
    outputs.append(path1);
    QPainterPath path2;
    path2.moveTo(  0,   0);
    path2.lineTo(- 4,   0);
    path2.lineTo(- 4,  10);
    outputs.append(path2);
    QPainterPath path3;
    path3.moveTo(  0,   0);
    path3.lineTo(-10,   0);
    path3.lineTo(-10,   4);
    outputs.append(path3);
    QPainterPath path4;
    path4.moveTo(  0,   0);
    path4.lineTo(  0, - 4);
    path4.lineTo(-10, - 4);
    outputs.append(path4);
    QPainterPath path5;
    path5.moveTo(  0,   0);
    path5.lineTo(  0, -10);
    path5.lineTo(- 4, -10);
    outputs.append(path5);
    QPainterPath path6;
    path6.moveTo(  0,   0);
    path6.lineTo(  4,   0);
    path6.lineTo(  4, -10);
    outputs.append(path6);
    QPainterPath path7;
    path7.moveTo(  0,   0);
    path7.lineTo( 10,   0);
    path7.lineTo( 10, - 4);
    outputs.append(path7);

    for (int i = 0; i < inputs.count(); i++)
    {
        QLineF input = inputs[i];
        QPainterPath expected = outputs[i];
        StraightPathRouter router;
        router.setDirection(StraightPathRouter::NegativeDirection);
        router.start(input.p1());
        QPainterPath actual = router.route(input.p2());
        if (actual != expected)
        {
            qDebug() << "SN FAIL" << i;
            qDebug() << "SN  input:" << input;
            qDebug() << "SN  actual:" << actual;
            qDebug() << "SN  expected:" << expected;
        }
        else
        {
            qDebug() << "SN PASS" << i;
        }
    }
}

void tst_DiagonalPathRouter_positiveDirection()
{
    QList<QPainterPath> outputs;
    QPainterPath path0;
    path0.moveTo(  0,   0);
    path0.lineTo(  6,   0);
    path0.lineTo( 10,   4);
    outputs.append(path0);
    QPainterPath path1;
    path1.moveTo(  0,   0);
    path1.lineTo(  4,   4);
    path1.lineTo(  4,  10);
    outputs.append(path1);
    QPainterPath path2;
    path2.moveTo(  0,   0);
    path2.lineTo(  0,   6);
    path2.lineTo(- 4,  10);
    outputs.append(path2);
    QPainterPath path3;
    path3.moveTo(  0,   0);
    path3.lineTo(- 4,   4);
    path3.lineTo(-10,   4);
    outputs.append(path3);
    QPainterPath path4;
    path4.moveTo(  0,   0);
    path4.lineTo( -6,   0);
    path4.lineTo(-10, - 4);
    outputs.append(path4);
    QPainterPath path5;
    path5.moveTo(  0,   0);
    path5.lineTo(- 4, - 4);
    path5.lineTo(- 4, -10);
    outputs.append(path5);
    QPainterPath path6;
    path6.moveTo(  0,   0);
    path6.lineTo(  0, - 6);
    path6.lineTo(  4, -10);
    outputs.append(path6);
    QPainterPath path7;
    path7.moveTo(  0,   0);
    path7.lineTo(  4, - 4);
    path7.lineTo( 10, - 4);
    outputs.append(path7);

    for (int i = 0; i < inputs.count(); i++)
    {
        QLineF input = inputs[i];
        QPainterPath expected = outputs[i];
        DiagonalPathRouter router;
        router.start(input.p1());
        QPainterPath actual = router.route(input.p2());
        if (actual != expected)
        {
            qDebug() << "DP FAIL" << i;
            qDebug() << "DP  input:" << input;
            qDebug() << "DP  actual:" << actual;
            qDebug() << "DP  expected:" << expected;
        }
        else
        {
            qDebug() << "DP PASS" << i;
        }
    }
}


void tst_DiagonalPathRouter_negativeDirection()
{
    QList<QPainterPath> outputs;
    QPainterPath path0;
    path0.moveTo(  0,   0);
    path0.lineTo(  4,   4);
    path0.lineTo( 10,   4);
    outputs.append(path0);
    QPainterPath path1;
    path1.moveTo(  0,   0);
    path1.lineTo(  0,   6);
    path1.lineTo(  4,  10);
    outputs.append(path1);
    QPainterPath path2;
    path2.moveTo(  0,   0);
    path2.lineTo(- 4,   4);
    path2.lineTo(- 4,  10);
    outputs.append(path2);
    QPainterPath path3;
    path3.moveTo(  0,   0);
    path3.lineTo(- 6,   0);
    path3.lineTo(-10,   4);
    outputs.append(path3);
    QPainterPath path4;
    path4.moveTo(  0,   0);
    path4.lineTo(- 4, - 4);
    path4.lineTo(-10, - 4);
    outputs.append(path4);
    QPainterPath path5;
    path5.moveTo(  0,   0);
    path5.lineTo(  0, - 6);
    path5.lineTo(- 4, -10);
    outputs.append(path5);
    QPainterPath path6;
    path6.moveTo(  0,   0);
    path6.lineTo(  4, - 4);
    path6.lineTo(  4, -10);
    outputs.append(path6);
    QPainterPath path7;
    path7.moveTo(  0,   0);
    path7.lineTo(  6,   0);
    path7.lineTo( 10, - 4);
    outputs.append(path7);

    for (int i = 0; i < inputs.count(); i++)
    {
        QLineF input = inputs[i];
        QPainterPath expected = outputs[i];
        DiagonalPathRouter router;
        router.setDirection(DiagonalPathRouter::NegativeDirection);
        router.start(input.p1());
        QPainterPath actual = router.route(input.p2());
        if (actual != expected)
        {
            qDebug() << "DN FAIL" << i;
            qDebug() << "DN  input:" << input;
            qDebug() << "DN  actual:" << actual;
            qDebug() << "DN  expected:" << expected;
        }
        else
        {
            qDebug() << "DN PASS" << i;
        }
    }
}
