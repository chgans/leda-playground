#ifndef POLARGRID_H
#define POLARGRID_H

#include "igrid.h"

// TODO: don't draw small cells (eg: inner circles and radial lines)

class PolarGrid : public IGrid
{
public:
    explicit PolarGrid();
    explicit PolarGrid(const PolarGrid &other);

    QString typeName() const;
    QString typeId() const;
    QString description() const;
    IGrid *clone() const;
    void draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const;
    QRectF rect() const;
    QPointF snap(const QSizeF &pixelPerMm, QPointF point) const;
    QPainterPath shape(const QSizeF &pixelPerMm) const;

    qreal angularStep() const { return mAngularStep; }
    void setAngularStep(qreal angle) { mAngularStep = angle; }

    qreal radialStep() const { return mRadialStep; }
    void setRadialStep(qreal step) { mRadialStep = step; }

    QPair<qreal, qreal> angularRange() const { return mAngularRange; }
    void setAngularRange(qreal start, qreal end) { mAngularRange = qMakePair(start, end); }

    QPair<qreal, qreal> radialRange() const { return mRadialRange; }
    void setRadialRange(qreal start, qreal end) { mRadialRange = qMakePair(start, end); }

protected:
    void drawPolarGrid(QSizeF pixelPerMm, QPainter *painter, const QColor &color, Qt::PenStyle style,
                       qreal radialStep, qreal angularStep) const;
    bool shouldDrawCoarse(const QSizeF &pixelPerMm) const;
    bool shouldDrawFine(const QSizeF &pixelPerMm) const;

    qreal mAngularStep;
    qreal mRadialStep;
    QPair<qreal, qreal> mAngularRange;
    QPair<qreal, qreal> mRadialRange;

};

#endif // POLARGRID_H
