#include "igrid.h"

IGrid::IGrid():
    mPriority(0),
    mCoarseMultiplier(5),
    mFineLineStyle(Qt::SolidLine),
    mFineLineColor(QColor("#4d4d5c")),
    mCoarseLineStyle(Qt::SolidLine),
    mCoarseLineColor(QColor("#918d90")),
    mEnabledForComponents(true)
{
}

IGrid::IGrid(const IGrid &other):
    mLabel(other.mLabel),
    mPriority(other.mPriority),
    mOrigin(other.mOrigin),
    mCoarseMultiplier(other.mCoarseMultiplier),
    mFineLineStyle(other.mFineLineStyle),
    mFineLineColor(other.mFineLineColor),
    mCoarseLineStyle(other.mCoarseLineStyle),
    mCoarseLineColor(other.mCoarseLineColor),
    mEnabledForComponents(other.mEnabledForComponents)
{

}

IGrid::~IGrid()
{

}

QBrush IGrid::backgroundBrush() const
{
    return Qt::NoBrush;
}
