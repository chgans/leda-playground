#include "ipathrouter.h"

IPathRouter::IPathRouter():
    m_direction(PositiveDirection)
{

}

IPathRouter::~IPathRouter()
{

}

IPathRouter::Direction IPathRouter::direction() const
{
    return m_direction;
}

void IPathRouter::setDirection(IPathRouter::Direction dir)
{
    m_direction = dir;
}

void IPathRouter::toggleDirection()
{
    if (m_direction == PositiveDirection)
    {
        m_direction = NegativeDirection;
    }
    else
    {
        m_direction = PositiveDirection;
    }
}

StraightPathRouter::StraightPathRouter(): IPathRouter()
{
}

StraightPathRouter::~StraightPathRouter()
{
}

void StraightPathRouter::start(const QPointF &from)
{
    if (m_from == from)
        return;
    m_from = from;
}

QPainterPath StraightPathRouter::route(const QPointF &to)
{
    qreal angle = QLineF(m_from, to).angle();
    int usecase = qFloor(angle/90.0) % 2;

    QPainterPath path;
    path.moveTo(m_from);
    if (direction() == PositiveDirection)
    {
        if (usecase == 1)
        {
            path.lineTo(to.x(), m_from.y());
        }
        else
        {
            path.lineTo(m_from.y(), to.y());
        }
    }
    else
    {
        if (usecase == 1)
        {
            path.lineTo(m_from.x(), to.y());
        }
        else
        {
            path.lineTo(to.x(), m_from.y());
        }
    }
    path.lineTo(to);
    return path;
}

DiagonalPathRouter::DiagonalPathRouter(): IPathRouter()
{
}

DiagonalPathRouter::~DiagonalPathRouter()
{
}

void DiagonalPathRouter::start(const QPointF &from)
{
    if (m_from == from)
        return;
    m_from = from;
}

QPainterPath DiagonalPathRouter::route(const QPointF &to)
{
    QPainterPath path;
    path.moveTo(m_from);

    qreal angle = QLineF(m_from, to).angle();
    int usecase = qFloor(angle/45.0) % 4;

    if (direction() == PositiveDirection)
    {
        switch (usecase)
        {
        case 0:
            path.lineTo(-to.y(), to.y());
            break;
        case 1:
            path.lineTo(0, to.x()+to.y());
            break;
        case 2:
            path.lineTo(to.x(), to.x());
            break;
        case 3:
            path.lineTo(to.x()-to.y(), 0);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (usecase) {
        case 0:
            path.lineTo(to.x()+to.y(), 0);
            break;
        case 1:
            path.lineTo(to.x(), -to.x());
            break;
        case 2:
            path.lineTo(0, to.y()-to.x());
            break;
        case 3:
            path.lineTo(to.y(), to.y());
            break;
        default:
            break;
        }
    }
    path.lineTo(to);
    return path;
}

DirectPathRouter::DirectPathRouter(): IPathRouter()
{

}

DirectPathRouter::~DirectPathRouter()
{

}

void DirectPathRouter::start(const QPointF &from)
{
    m_from = from;
}

QPainterPath DirectPathRouter::route(const QPointF &to)
{
    QPainterPath path;
    path.moveTo(m_from);
    path.lineTo(to);
    return path;
}



/****************************************************************************
 *
 ****************************************************************************/

#include <QDebug>

/*
 x°
90° HxV (α>0)
90° VxH (α<0)
45° HOV (α>0)
45° VOH (α<0)
*/
// H,O,H and V,O,V
QPainterPath routeToPoint(const QPointF &from, const QPointF &to)
{
    QPointF vector = to - from;
    qreal dx = vector.x();
    qreal dy = vector.y();

    bool normaliseX = dx < 0;
    if (normaliseX)
    {
        dx = -dx;
    }
    bool normaliseY = dy < 0;
    if (normaliseY)
    {
        dy = -dy;
    }
    bool swapXY = dy > dx;
    if (swapXY)
    {
        qSwap(dx, dy);
    }

    qreal x1 = (dx-dy)/2.0;
    qreal y1 = 0;
    qreal x2 = x1+dy;
    qreal y2 = dy;

    if (swapXY)
    {
        qSwap(x1, y1);
        qSwap(x2, y2);
    }
    if (normaliseY)
    {
        y1 = -y1;
        y2 = -y2;
    }
    if (normaliseX)
    {
        x1 = -x1;
        x2 = -x2;
    }

    QPainterPath path;
    path.moveTo(0, 0);
    path.lineTo(x1, y1);
    path.lineTo(x2, y2);
    path.translate(from);
    path.lineTo(to);

    return path;
}

void tst_routeToPoint()
{
    QList<QLineF> inputs;
    QList<QPainterPath> outputs;

    QLineF line1(0, 0, 10, 4);
    QPainterPath path1;
    path1.lineTo(3, 0);
    path1.lineTo(7, 4);
    path1.lineTo(10, 4);
    inputs.append(line1);
    outputs.append(path1);

    QLineF line2(0, 0, 4, 10);
    QPainterPath path2;
    path2.lineTo(0, 3);
    path2.lineTo(4, 7);
    path2.lineTo(4, 10);
    inputs.append(line2);
    outputs.append(path2);

    QLineF line3(0, 0, -4, 10);
    QPainterPath path3;
    path3.lineTo(0, 3);
    path3.lineTo(-4, 7);
    path3.lineTo(-4, 10);
    inputs.append(line3);
    outputs.append(path3);

    QLineF line4(0, 0, -10, 4);
    QPainterPath path4;
    path4.lineTo(-3, 0);
    path4.lineTo(-7, 4);
    path4.lineTo(-10, 4);
    inputs.append(line4);
    outputs.append(path4);

    QLineF line5(0, 0, -10, -4);
    QPainterPath path5;
    path5.lineTo(-3, 0);
    path5.lineTo(-7, -4);
    path5.lineTo(-10, -4);
    inputs.append(line5);
    outputs.append(path5);

    QLineF line6(0, 0, -4, -10);
    QPainterPath path6;
    path6.lineTo(0, -3);
    path6.lineTo(-4, -7);
    path6.lineTo(-4, -10);
    inputs.append(line6);
    outputs.append(path6);

    QLineF line7(0, 0, 4, -10);
    QPainterPath spath7;
    spath7.lineTo(0, -3);
    spath7.lineTo(4, -7);
    spath7.lineTo(4, -10);
    inputs.append(line7);
    outputs.append(spath7);

    QLineF line8(0, 0, 10, -4);
    QPainterPath path8;
    path8.lineTo(3, 0);
    path8.lineTo(7, -4);
    path8.lineTo(10, -4);
    inputs.append(line8);
    outputs.append(path8);

    inputs.append(line8.translated(3.14, 15.92));
    outputs.append(path8.translated(3.14, 15.92));
    inputs.append(line8.translated(-3.14, 15.92));
    outputs.append(path8.translated(-3.14, 15.92));
    inputs.append(line8.translated(3.14, -15.92));
    outputs.append(path8.translated(3.14, -15.92));
    inputs.append(line8.translated(-3.14, -15.92));
    outputs.append(path8.translated(-3.14, -15.92));

    for (int i=0; i<inputs.count(); i++)
    {
        QLineF input = inputs.at(i);
        QPainterPath expected = outputs.at(i);
        QPainterPath actual = routeToPoint(input.p1(), input.p2());
        if (actual != expected)
        {
            qDebug() << "FAIL" << i;
            qDebug() << " actual:" << actual;
            qDebug() << " expected:" << expected;
        }
        else
        {
            qDebug() << "PASS" << i;
        }
    }
}
