#ifndef CARTESIANGRID_H
#define CARTESIANGRID_H

#include "igrid.h"
#include <QPointF>
#include <QSizeF>

class CartesianGrid : public IGrid
{
public:
    enum Quadrant {
        TopLeftQuadrant = 0,
        TopRightQuadrant = 1,
        BottomRightQuadrant = 2,
        BottomLeftQuadrant = 3
    };

    explicit CartesianGrid();
    explicit CartesianGrid(const CartesianGrid &other);

    QString typeName() const;
    QString typeId() const;
    QString description() const;
    IGrid *clone() const;
    void draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const;
    QRectF rect() const;
    QPointF snap(const QSizeF &pixelPerMm, QPointF point) const;
    QPainterPath shape(const QSizeF &pixelPerMm) const;

    qreal rotation() const { return mRotation; }
    void setRotation(qreal angle) { mRotation = angle; }

    QSizeF size() const { return mSize; }
    void setSize(QSizeF size) { mSize = size; }

    QPointF step() const { return mStep; }
    void setStep(QPointF step) { mStep = step; }

    bool isQuadrantEnabled(Quadrant quadrant) const { return mQuadrants[quadrant]; }
    void setQuadrantEnabled(Quadrant quadrant, bool enabled) { mQuadrants[quadrant] = enabled; }


protected:
    void drawCartGrid(QPainter *painter, const QColor &color, Qt::PenStyle style,
                      const QRectF &rect, qreal xstep, qreal ystep) const;
    bool shouldDrawCoarse(const QSizeF &pixelPerMm) const;
    bool shouldDrawFine(const QSizeF &pixelPerMm) const;

    qreal mRotation;
    QSizeF mSize;
    QPointF mStep;
    bool mQuadrants[4];
};

#endif // CARTESIANGRID_H
