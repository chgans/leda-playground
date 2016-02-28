#ifndef IPATHROUTER_H
#define IPATHROUTER_H

#include <QPainterPath>
#include <QLineF>
#include <QtMath>

class IPathRouter
{
public:

    enum Direction
    {
        PositiveDirection = 0,
        NegativeDirection = 1
    };

    IPathRouter();
    virtual ~IPathRouter();

    virtual void start(const QPointF &from) = 0;
    virtual QPainterPath route(const QPointF &to) = 0;

    Direction direction() const;
    void setDirection(Direction dir);
    void toggleDirection();

private:
    Direction m_direction;
};

class DirectPathRouter: public IPathRouter
{

public:

    DirectPathRouter();
    ~DirectPathRouter();

    void start(const QPointF &from);
    QPainterPath route(const QPointF &to);

private:
    QPointF m_from;
};

/*
 * Right   Left (Width > Height)
 * x----   x
 *      |  |
 *      x   ----x
 *
 * Right   Left  (Width < Height)
 * x--       x
 *    |      |
 *    |      |
 *    x       --x
 *
 */
class StraightPathRouter: public IPathRouter
{
public:

    StraightPathRouter();
    ~StraightPathRouter();

    void start(const QPointF &from);
    QPainterPath route(const QPointF &to);

private:
    QPointF m_from;
};

/*
 * Right   Left (Width > Height)
 * x--     x
 *    \     \
 *     x     --x
 *
 * Right   Left  (Width < Height)
 * x       x
 *  \      |
 *   \      \
 *    |      \
 *    x       x
 *
 */
class DiagonalPathRouter: public IPathRouter
{

public:

    DiagonalPathRouter();
    ~DiagonalPathRouter();

    void start(const QPointF &from);
    QPainterPath route(const QPointF &to);

private:
    QPointF m_from;
};



#endif // IPATHROUTER_H
