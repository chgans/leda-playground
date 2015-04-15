#include "designlayerset.h"

#include "designlayer.h"

DesignLayerSet::DesignLayerSet(QObject *parent):
    QObject(parent),
    m_builtIn(false)
{

}

DesignLayerSet::~DesignLayerSet()
{

}

QString DesignLayerSet::name() const
{
    return m_name;
}

void DesignLayerSet::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}

QString DesignLayerSet::customName() const
{
    return m_customName;
}

QString DesignLayerSet::effectiveName() const
{
    if (m_customName.isEmpty())
        return m_name;
    return m_customName;
}

void DesignLayerSet::setCustomName(const QString &name)
{
    if (m_customName == name)
        return;

    m_customName = name;
}

bool DesignLayerSet::isBuiltIn() const
{
    return m_builtIn;
}

bool DesignLayerSet::contains(DesignLayer *layer) const
{
    return m_layers.contains(layer);
}

QList<DesignLayer *> DesignLayerSet::allLayers()
{
    return m_layers;
}

void DesignLayerSet::setBuiltIn(bool builtIn)
{
    if (m_builtIn == builtIn)
        return;

    m_builtIn = builtIn;
}

void DesignLayerSet::setType(int type)
{
    if (m_type == type)
        return;
    m_type = type;
}

QList<DesignLayer *> DesignLayerSet::enabledLayers()
{
    QList<DesignLayer *> list;
    foreach (DesignLayer *layer, m_layers) {
        if (layer->isEnabled())
            list << layer;
    }
    return list;
}

int DesignLayerSet::layerCount()
{
    return m_layers.count();
}

void DesignLayerSet::add(DesignLayer *layer)
{
    m_layers.append(layer);
}

void DesignLayerSet::add(QList<DesignLayer *> layers)
{
    m_layers.append(layers);
}

void DesignLayerSet::remove(DesignLayer *layer)
{
    m_layers.removeOne(layer);
}

