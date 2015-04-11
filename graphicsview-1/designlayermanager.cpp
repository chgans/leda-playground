#include "designlayermanager.h"

// FIXME: use SOFT_ASSERT in some places to generate warning/error messages

DesignLayerManager::DesignLayerManager(QObject *parent) : QObject(parent)
{

}

DesignLayerManager *DesignLayerManager::m_instance = nullptr;

DesignLayerManager *DesignLayerManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new DesignLayerManager();

    return m_instance;
}

DesignLayerManager::~DesignLayerManager()
{

}

void DesignLayerManager::loadFromSettings(const QSettings &settings)
{
    Q_UNUSED(settings);
}

void DesignLayerManager::saveToSettings(QSettings *settings) const
{
    Q_UNUSED(settings);
}

void DesignLayerManager::loadFromDefaults()
{
    int stackPosition = 0;
    DesignLayer *topLayer;
    DesignLayer *bottomLayer;

    for (DesignLayer::LayerSet set = DesignLayer::_BeginLayerSet;
         set < DesignLayer::_EndLayerSet;
         set = DesignLayer::LayerSet(set + 1)) {
        m_layerSetMap[set] = DesignLayerList();
        switch (set) {
        case DesignLayer::SignalLayerSet:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(set, DesignLayer::SignalCategory, stackPosition + i);
                bottomLayer = addLayer(set, DesignLayer::SignalCategory, stackPosition + 32 - 1 - i);
                if (i == 0) {
                    topLayer->setName(QString("Top Layer"));
                    bottomLayer->setName(QString("Bottom Layer"));
                    topLayer->setVisible(true);
                    bottomLayer->setVisible(true);
                }
                else {
                    topLayer->setName(QString("Mid Layer %1").arg(i + 1)); // 1 based
                    bottomLayer->setName(QString("Mid Layer %1").arg(32 - i)); // 1 based
                    topLayer->setVisible(false);
                    bottomLayer->setVisible(false);
                }
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
            }
            stackPosition += 32;
            break;
        case DesignLayer::PlaneLayerSet:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(set, DesignLayer::PlaneCategory, stackPosition + i);
                bottomLayer = addLayer(set, DesignLayer::PlaneCategory, stackPosition + 32 - 1 - i);
                topLayer->setName(QString("Internal Plane %1").arg(i + 1)); // 1 based
                bottomLayer->setName(QString("Internal Plane %1").arg(32 - i)); // 1 based
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setVisible(false);
                bottomLayer->setVisible(false);
            }
            stackPosition += 32;
            break;
        case DesignLayer::MaskLayerSet:
            topLayer = addLayer(set, DesignLayer::MaskCategory, stackPosition++);
            topLayer->setName("Top Solder");
            bottomLayer = addLayer(set, DesignLayer::MaskCategory, stackPosition++);
            bottomLayer->setName("Bottom Solder");
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
            topLayer = addLayer(set, DesignLayer::MaskCategory, stackPosition++);
            topLayer->setName("Top Paste");
            bottomLayer = addLayer(set, DesignLayer::MaskCategory, stackPosition++);
            bottomLayer->setName("Bottom Paste");
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
            break;
        case DesignLayer::SilkscreenLayerSet:
            topLayer = addLayer(set, DesignLayer::SilkscreenCategory, stackPosition++);
            topLayer->setName("Top Overlay");
            bottomLayer = addLayer(set, DesignLayer::SilkscreenCategory, stackPosition++);
            bottomLayer->setName("Bottom Overlay");
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
            break;
        case DesignLayer::MechanicalLayerSet:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(set, DesignLayer::MechanicalCategory, stackPosition + i);
                bottomLayer = addLayer(set, DesignLayer::MechanicalCategory, stackPosition + 32 - 1 - i);
                topLayer->setName(QString("Mechanical %1").arg(i + 1)); // 1 based
                bottomLayer->setName(QString("Mechanical %1").arg(32 - i)); // 1 based
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setVisible(false);
                bottomLayer->setVisible(false);
            }
            stackPosition += 32;
            break;
        default:
            break;
        }
    }
}

int DesignLayerManager::layerCount() const
{
    return m_layerMap.count();
}

int DesignLayerManager::layerSetCount() const
{
    return m_layerSetMap.count();
}

int DesignLayerManager::customLayerSetCount() const
{
    return m_customLayerSetMap.count();
}

DesignLayerList DesignLayerManager::layerSet(DesignLayer::LayerSet set) const
{
    Q_ASSERT(m_layerSetMap.contains(int(set)));
    return m_layerSetMap[int(set)];
}

DesignLayerList DesignLayerManager::customLayerSet(int id) const
{
    Q_ASSERT(m_customLayerSetMap.contains(id));
    return m_customLayerSetMap[id];
}

DesignLayerList DesignLayerManager::allLayers() const
{
    return m_layerMap.values();
}

DesignLayer *DesignLayerManager::layerAt(int stackPosition) const
{
    Q_ASSERT(m_layerMap.contains(stackPosition));
    return m_layerMap[stackPosition];
}

void DesignLayerManager::setLayerEnabled(int stackPosition, bool enabled)
{
    Q_ASSERT(m_layerMap.contains(stackPosition));
    if (m_layerMap[stackPosition]->isVisible() == enabled)
        return;
    m_layerMap[stackPosition]->setVisible(enabled);
    emit layerEnabledChanged(stackPosition, enabled);
}

bool DesignLayerManager::isLayerEnabled(int stackPosition) const
{
    Q_ASSERT(m_layerMap.contains(stackPosition));
    return m_layerMap[stackPosition]->isVisible();
}

DesignLayerList DesignLayerManager::enabledLayers() const
{
    DesignLayerList list;
    foreach (DesignLayer *layer, m_layerMap.values()) {
        if (layer->isVisible())
            list.append(layer);
    }
    return list;
}

// TODO: What do we do with names?
DesignLayerList DesignLayerManager::addCustomLayerSet(int id, const QString &name)
{
    Q_UNUSED(name)
    Q_ASSERT(!m_customLayerSetMap.contains(id));
    m_customLayerSetMap[id] = DesignLayerList();
    return m_customLayerSetMap[id];
}

void DesignLayerManager::removeCustomLayerSet(int id)
{
    // FIXME: use SOFT_ASSERT to generate warning/error messages
    Q_ASSERT(m_customLayerSetMap.contains(id));
    m_customLayerSetMap.remove(id);
}

// TODO: private?
DesignLayer *DesignLayerManager::addLayer(DesignLayer::LayerSet set, DesignLayer::Category category, int stackPosition)
{
    Q_ASSERT(!m_layerMap.contains(stackPosition));
    Q_ASSERT(m_layerSetMap.contains(int(set)));
    DesignLayer *layer = new DesignLayer();
    layer->setLayerSet(set);
    layer->setCategory(category);
    layer->setStackPosition(stackPosition);
    m_layerMap[stackPosition] = layer;
    m_layerSetMap[int(set)].append(layer);
    emit layerAdded(layer);
    return layer;
}

// TODO: private?
// TODO: custom layer sets
void DesignLayerManager::removeLayer(DesignLayer *layer)
{
    Q_ASSERT(m_layerMap.values().contains(layer));
    Q_ASSERT(m_layerMap.contains(layer->stackPosition()));
    Q_ASSERT(m_layerSetMap.contains(layer->layerSet()));
    m_layerMap.remove(layer->stackPosition());
    m_layerSetMap[layer->layerSet()].removeOne(layer);
}

