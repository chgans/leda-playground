#ifndef IGRID_H
#define IGRID_H

#include <QString>
#include <QPointF>
#include <QColor>
#include <QBrush>
#include <QRectF>
#include <QPainterPath>

class QPainter;

class IGrid
{
public:

    explicit IGrid();
    explicit IGrid(const IGrid &other);
    virtual ~IGrid();

    QString label() const { return mLabel; }
    void setLabel(const QString &label) { mLabel = label; }

    virtual QString typeName() const = 0;
    virtual QString typeId() const = 0; // FIXME: see Factory.typeId()
    virtual QString description() const = 0;

    virtual IGrid *clone() const = 0;

    virtual void draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const = 0;
    virtual QRectF rect() const = 0;
    virtual QPainterPath shape(const QSizeF &pixelPerMm) const = 0;

    virtual QPointF snap(const QSizeF &pixelPerMm, QPointF point) const = 0;

    int priority() const { return mPriority; }
    void setPriority(int prio) { mPriority = prio; }

    QPointF origin() const { return mOrigin; }
    void setOrigin(QPointF p) { mOrigin = p; }

    qreal coarseMultiplier() const { return mCoarseMultiplier; }
    void setCoarseMultiplier(qreal m) { mCoarseMultiplier = m; }

    Qt::PenStyle fineLineStyle() const { return mFineLineStyle; }
    void setFineLineStyle(Qt::PenStyle style) { mFineLineStyle = style; }

    QColor fineLineColor() const { return mFineLineColor; }
    void setFineLineColor(QColor color) { mFineLineColor = color; }

    Qt::PenStyle coarseLineStyle() const { return mCoarseLineStyle; }
    void setCoarseLineStyle(Qt::PenStyle style) { mCoarseLineStyle = style; }

    QColor coarseLineColor() const { return mCoarseLineColor; }
    void setCoarseLineColor(QColor color) { mCoarseLineColor = color; }

    // FIXME: snap stuff here
    bool enabledForComponents() const { return mEnabledForComponents; }
    void setEnabledForComponents(bool enabled) { mEnabledForComponents = enabled; }

    QBrush backgroundBrush() const;

private:
    QString mLabel;
    int mPriority;
    QPointF mOrigin;
    qreal mCoarseMultiplier;
    Qt::PenStyle mFineLineStyle;
    QColor mFineLineColor;
    Qt::PenStyle mCoarseLineStyle;
    QColor mCoarseLineColor;
    bool mEnabledForComponents;
};

#endif // IGRID_H
