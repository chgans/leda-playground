#include "cartesiangrid.h"

#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

#include <QDebug>

//
static const qreal MIN_SIZE_IN_PIXEL = 10;

CartesianGrid::CartesianGrid():
    IGrid(),
    mRotation(0),
    mSize(100, 100),
    mStep(10, 10)
{
    for (int i=0; i<4; ++i)
        mQuadrants[i] = true;
}

CartesianGrid::CartesianGrid(const CartesianGrid &other):
    IGrid(other),
    mRotation(other.mRotation),
    mSize(other.mSize),
    mStep(other.mStep)
{
    for (int i=0; i<4; ++i)
        mQuadrants[i] = other.mQuadrants[i];
}

QString CartesianGrid::typeName() const
{
    return "Cartesian";
}

QString CartesianGrid::typeId() const
{
    // FIXME: same as in factory
    return QStringLiteral("LibreEDA.Grid.Cartesian");
}

QString CartesianGrid::description() const
{
    return QString("Origin(%1, %2), Size(%3, %4), Steps(%5, %6)")
            .arg(origin().x()).arg(origin().y())
            .arg(size().width()).arg(size().height())
            .arg(step().x()).arg(step().y());
}

IGrid *CartesianGrid::clone() const
{
    CartesianGrid *grid = new CartesianGrid(*this);
    return grid;
}

void CartesianGrid::draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &prect) const
{
    //qDebug() << __PRETTY_FUNCTION__ << this << prect;

    QPainterPath ppath;
    ppath.addRect(prect);
    //painter->fillPath(shape().intersected(ppath), backgroundBrush());

    bool drawCoarse = shouldDrawCoarse(pixelPerMm);
    if (!drawCoarse)
        return;
    bool drawFine = shouldDrawFine(pixelPerMm);

    //painter->setClipRect(prect); // FIXME: get rid of this clip
    if (mQuadrants[0]) {
        QRectF r = QRectF(rect().topLeft(),
                          rect().center()).intersected(prect);
        if (!r.isEmpty()) {
#if 0
            qreal xmod = fmod(r.left() - origin().x(), step().x());
            qreal ymod = fmod(r.top() - origin().y(), step().y());
            r.setLeft(r.left() - xmod);
            r.setTop(r.top() - ymod);
#elif 0
            r.setLeft(int((r.left() - origin().x())/step().x()) * step().x() + origin().x());
            r.setTop (int((r.top()  - origin().y())/step().y()) * step().y() + origin().y());
#endif
            if (drawFine)
                drawCartGrid(painter, fineLineColor(), fineLineStyle(),
                             r, step().x()/coarseMultiplier(), step().y()/coarseMultiplier());
            drawCartGrid(painter, coarseLineColor(), coarseLineStyle(),
                         r, step().x(), step().y());
        }
    }
    if (mQuadrants[1]) {
        QRectF r = QRectF(QPointF(rect().center().x(), rect().top()),
                          QPointF(rect().right(), rect().center().y())).intersected(prect);
        if (!r.isEmpty()) {
            if (drawFine)
                drawCartGrid(painter, fineLineColor(), fineLineStyle(),
                             r, step().x()/coarseMultiplier(), step().y()/coarseMultiplier());
            drawCartGrid(painter, coarseLineColor(), coarseLineStyle(),
                         r, step().x(), step().y());
        }
    }
    if (mQuadrants[2]) {
        QRectF r = QRectF(rect().center(),
                          rect().bottomRight()).intersected(prect);
        if (!r.isEmpty()) {
            if (drawFine)
                drawCartGrid(painter, fineLineColor(), fineLineStyle(),
                             r, step().x()/coarseMultiplier(), step().y()/coarseMultiplier());
            drawCartGrid(painter, coarseLineColor(), coarseLineStyle(),
                         r, step().x(), step().y());
        }
    }
    if (mQuadrants[3]) {
        QRectF r = QRectF(QPointF(rect().left(), rect().center().y()),
                          QPointF(rect().center().x(), rect().bottom())).intersected(prect);
        if (!r.isEmpty()) {
            if (drawFine)
                drawCartGrid(painter, fineLineColor(), fineLineStyle(),
                             r, step().x()/coarseMultiplier(), step().y()/coarseMultiplier());
            drawCartGrid(painter, coarseLineColor(), coarseLineStyle(),
                         r, step().x(), step().y());
        }
    }
}

QRectF CartesianGrid::rect() const
{
    return QRectF(origin(), size());
}

QPointF CartesianGrid::snap(const QSizeF &pixelPerMm, QPointF point) const
{
    if (point.isNull() || step().isNull())
        return QPointF();

    qreal xstep = step().x();
    qreal ystep = step().y();
    if (shouldDrawFine(pixelPerMm)) {
        xstep /= coarseMultiplier();
        ystep /= coarseMultiplier();
    }

    qreal x = qRound((point.x() - origin().x())/ xstep) * xstep + origin().x();
    qreal y = qRound((point.y() - origin().y())/ ystep) * ystep + origin().y();
    return QPointF(x, y);
}

QPainterPath CartesianGrid::shape(const QSizeF &pixelPerMm) const
{
    QPainterPath path;

    if (!shouldDrawCoarse(pixelPerMm))
        return path;

    if (mQuadrants[0])
        path.addRect(QRectF(rect().topLeft(),
                            rect().center()));
    if (mQuadrants[1])
        path.addRect(QRectF(QPointF(rect().center().x(), rect().top()),
                            QPointF(rect().right(), rect().center().y())));
    if (mQuadrants[2])
        path.addRect(QRectF(rect().center(),
                            rect().bottomRight()));
    if (mQuadrants[3])
        path.addRect(QRectF(QPointF(rect().left(), rect().center().y()),
                            QPointF(rect().center().x(), rect().bottom())));
    return path;
}

void CartesianGrid::drawCartGrid(QPainter *painter, const QColor &color, Qt::PenStyle style,
                                 const QRectF &rect, qreal xstep, qreal ystep) const
{
    qreal left = int((rect.left() - origin().x())/step().x()) * step().x() + origin().x();
    qreal top = int((rect.top()  - origin().y())/step().y()) * step().y() + origin().y();
    qreal right = rect.right();
    qreal bottom = rect.bottom();

    painter->setPen(QPen(color, 0, style));
    if (style == Qt::SolidLine) {
        QVarLengthArray<QLineF, 1024> lines;
        for (qreal x = left; x <= right; x += xstep) {
            lines.append(QLineF(x, top,
                                x, bottom));
        }
        for (qreal y = top; y <= bottom; y += ystep) {
            lines.append(QLineF(left, y,
                                right, y));
        }
        painter->drawLines(lines.data(), lines.size());
    }
    else if (style == Qt::DotLine) {
        QVarLengthArray<QPointF, 1024> points;
        for (qreal x = left; x <= right; x += xstep) {
            for (qreal y = top; y <= bottom; y += ystep) {
                points.append(QPointF(x, y));
            }
        }
        painter->drawPoints(points.data(), points.size());
    }
}

bool CartesianGrid::shouldDrawCoarse(const QSizeF &pixelPerMm) const
{
    return pixelPerMm.width()*step().x() > MIN_SIZE_IN_PIXEL &&
            pixelPerMm.height()*step().y() > MIN_SIZE_IN_PIXEL;
}

bool CartesianGrid::shouldDrawFine(const QSizeF &pixelPerMm) const
{
    return (pixelPerMm.width()*step().x()/coarseMultiplier()) > MIN_SIZE_IN_PIXEL &&
            (pixelPerMm.height()*step().y()/coarseMultiplier()) > MIN_SIZE_IN_PIXEL;
}
