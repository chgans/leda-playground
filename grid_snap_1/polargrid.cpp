#include "polargrid.h"

#include <QPainter>
#include <QPainterPath>
#include <QSizeF>

#include <QDebug>

static const int MIN_SIZE_IN_PIXEL = 10;

PolarGrid::PolarGrid():
    IGrid(),
    mAngularStep(10),
    mRadialStep(10),
    mAngularRange(qMakePair(0, 360)),
    mRadialRange(qMakePair(0, 100))
{
    setOrigin(QPointF(200, 200));
}

PolarGrid::PolarGrid(const PolarGrid &other):
    IGrid(other),
    mAngularStep(other.mAngularStep),
    mRadialStep(other.mRadialStep),
    mAngularRange(other.mAngularRange),
    mRadialRange(other.mRadialRange)
{

}

QString PolarGrid::typeName() const
{
    return QString("Polar");
}

QString PolarGrid::typeId() const
{
    return QStringLiteral("LibreEDA.Grid.Polar");
}

QString PolarGrid::description() const
{
    return QString("Origin(%1, %2), Angle(%3, %4, %5), Radius(%6, %7, %8)")
            .arg(origin().x()).arg(origin().y())
            .arg(angularRange().first).arg(angularRange().second).arg(angularStep())
            .arg(radialRange().first).arg(radialRange().second).arg(radialStep());
}

IGrid *PolarGrid::clone() const
{
    PolarGrid *grid = new PolarGrid(*this);
    return grid;
}

// TODO: always draw the radial line at mAngularRange.second
void PolarGrid::draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const
{
    if (!shouldDrawCoarse(pixelPerMm))
        return;

    painter->save();
    if (shouldDrawFine(pixelPerMm))
        drawPolarGrid(pixelPerMm, painter, fineLineColor(), fineLineStyle(),
                      mRadialStep/coarseMultiplier(), mAngularStep/coarseMultiplier());
    drawPolarGrid(pixelPerMm, painter, coarseLineColor(), coarseLineStyle(),
                  mRadialStep, mAngularStep);
    painter->restore();
}

void PolarGrid::drawPolarGrid(QSizeF pixelPerMm,
                              QPainter *painter, const QColor &color, Qt::PenStyle style,
                              qreal radialStep, qreal angularStep) const
{
    painter->setPen(QPen(color, 0, style));
    if (style == Qt::SolidLine) {
        qreal angle = 16.0*mAngularRange.first;
        qreal sweep = 16.0*(mAngularRange.second - mAngularRange.first);
        for (qreal r = mRadialRange.first; r <= mRadialRange.second; r+= radialStep) {
            QRectF rect(origin().x() - r, origin().y() - r,
                        2.0*r, 2.0*r);
            painter->drawArc(rect, angle, sweep);
        }
    }

    // Don't draw inner cells if thet are too small
    qreal minRadius = mRadialRange.second;
    for (qreal r = mRadialRange.first; r <= mRadialRange.second; r+= radialStep) {
        QLineF line = QLineF::fromPolar(r, angularStep).translated(origin());
        if (pixelPerMm.width()*qAbs(line.dx()) > MIN_SIZE_IN_PIXEL &&
                pixelPerMm.height()*qAbs(line.dy()) > MIN_SIZE_IN_PIXEL) {
            minRadius = r;
            if (!qFuzzyCompare(r, mRadialRange.first))
                 minRadius -= radialStep;
            break;
        }
    }

    for (qreal a = mAngularRange.first; a <= mAngularRange.second; a+= angularStep) {
        QLineF fullLine = QLineF::fromPolar(mRadialRange.second, a).translated(origin());
        if (style == Qt::SolidLine && minRadius < mRadialRange.second) {
            QPointF p1 = fullLine.pointAt(minRadius/mRadialRange.second);
            QLineF line(p1,
                        fullLine.p2());
            painter->drawLine(line);
        }
        else if (style == Qt::DotLine) {
            for (qreal r = minRadius; r <= mRadialRange.second; r+= radialStep) {
                painter->drawPoint(fullLine.pointAt(r/mRadialRange.second));
            }
        }
    }
}

bool PolarGrid::shouldDrawCoarse(const QSizeF &pixelPerMm) const
{
    return pixelPerMm.width()*mRadialStep > MIN_SIZE_IN_PIXEL;
}

bool PolarGrid::shouldDrawFine(const QSizeF &pixelPerMm) const
{
    return pixelPerMm.width()*mRadialStep/coarseMultiplier() > MIN_SIZE_IN_PIXEL;
}

QRectF PolarGrid::rect() const
{
    return QRectF(origin().x() - radialRange().second,
                  origin().y() - radialRange().second,
                  2.0*radialRange().second,
                  2.0*radialRange().second);
}


QPointF PolarGrid::snap(const QSizeF &pixelPerMm, QPointF point) const
{
    if (point.isNull())
        return QPointF();
    QLineF line(origin(), point);
    qreal angularStep = mAngularStep;
    qreal radialStep = mRadialStep;
    if (shouldDrawFine(pixelPerMm)) {
        angularStep /= coarseMultiplier();
        radialStep /= coarseMultiplier();
    }

    // FIXME: snap to the same grid we drew
    line.setAngle(qRound((line.angle()-mAngularRange.first)/angularStep) * angularStep + mAngularRange.first);
    line.setLength(qRound((line.length()-mRadialRange.first)/radialStep) * radialStep + mRadialRange.first);
    return line.p2();
}

QPainterPath PolarGrid::shape(const QSizeF &pixelPerMm) const
{
    if (!shouldDrawCoarse(pixelPerMm))
        return QPainterPath();

    QRectF outRect = QRectF(origin().x() - radialRange().second,
                            origin().y() - radialRange().second,
                            2.0*radialRange().second,
                            2.0*radialRange().second);
    QRectF inRect = QRectF(origin().x() - radialRange().first,
                           origin().y() - radialRange().first,
                           2.0*radialRange().first,
                           2.0*radialRange().first);

    QPainterPath outPath;
    outPath.moveTo(origin());
    outPath.arcTo(outRect, mAngularRange.first, mAngularRange.second - mAngularRange.first);
    outPath.closeSubpath();

    QPainterPath inPath;
    inPath.moveTo(origin());
    inPath.arcTo(inRect, mAngularRange.first, mAngularRange.second - mAngularRange.first);
    inPath.closeSubpath();
    return outPath.subtracted(inPath);
}
