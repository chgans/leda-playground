#include "pcbphysicallayer.h"

PcbPhysicalLayer::PcbPhysicalLayer()
{
}

PcbPhysicalLayer::PcbPhysicalLayer(const PcbPhysicalLayer &other)
{

}

PcbPhysicalLayer::~PcbPhysicalLayer()
{

}
qreal PcbPhysicalLayer::thickness() const
{
    return mThickness;
}

void PcbPhysicalLayer::setThickness(const qreal &value)
{
    mThickness = value;
}
QString PcbPhysicalLayer::material() const
{
    return mMaterial;
}

void PcbPhysicalLayer::setMaterial(const QString &value)
{
    mMaterial = value;
}

qreal PcbPhysicalConductorLayer::pullBackDistance() const
{
    return mPullBackDistance;
}

void PcbPhysicalConductorLayer::setPullBackDistance(const qreal &value)
{
    mPullBackDistance = value;
}

PcbPhysicalSignalLayer::ComponentPolicy PcbPhysicalSignalLayer::componentPolicy() const
{
    return mComponentPolicy;
}

void PcbPhysicalSignalLayer::setComponentPolicy(const ComponentPolicy &value)
{
    mComponentPolicy = value;
}

QString PcbPhysicalDielectricLayer::dielectricMaterial() const
{
    return mDielectricMaterial;
}

void PcbPhysicalDielectricLayer::setDielectricMaterial(const QString &value)
{
    mDielectricMaterial = value;
}
QString PcbPhysicalDielectricLayer::dielectricConstant() const
{
    return mDielectricConstant;
}

void PcbPhysicalDielectricLayer::setDielectricConstant(const QString &value)
{
    mDielectricConstant = value;
}

