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
    return m_thickness;
}

void PcbPhysicalLayer::setThickness(const qreal &value)
{
    m_thickness = value;
}
QString PcbPhysicalLayer::material() const
{
    return m_material;
}

void PcbPhysicalLayer::setMaterial(const QString &value)
{
    m_material = value;
}

qreal PcbPhysicalConductorLayer::pullBackDistance() const
{
    return m_pullBackDistance;
}

void PcbPhysicalConductorLayer::setPullBackDistance(const qreal &value)
{
    m_pullBackDistance = value;
}

PcbPhysicalSignalLayer::ComponentPolicy PcbPhysicalSignalLayer::componentPolicy() const
{
    return m_componentPolicy;
}

void PcbPhysicalSignalLayer::setComponentPolicy(const ComponentPolicy &value)
{
    m_componentPolicy = value;
}

QString PcbPhysicalDielectricLayer::dielectricMaterial() const
{
    return m_dielectricMaterial;
}

void PcbPhysicalDielectricLayer::setDielectricMaterial(const QString &value)
{
    m_dielectricMaterial = value;
}
QString PcbPhysicalDielectricLayer::dielectricConstant() const
{
    return m_dielectricConstant;
}

void PcbPhysicalDielectricLayer::setDielectricConstant(const QString &value)
{
    m_dielectricConstant = value;
}

