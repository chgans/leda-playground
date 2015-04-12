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

    for (DesignLayer::Category category = DesignLayer::SignalCategory;
         category <= DesignLayer::OtherCategory;
         category = DesignLayer::Category(category + 1)) {

        m_layerCategoryMap[category] = DesignLayerList();

        switch (category) {
        case DesignLayer::SignalCategory:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(layerName(category, i));
                bottomLayer->setName(layerName(category, 32 - 1 - i));
                if (i == 0) {
                    topLayer->setVisible(true);
                    bottomLayer->setVisible(true);
                }
                else {
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
        case DesignLayer::PlaneCategory:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(layerName(category, i));
                bottomLayer->setName(layerName(category, 32 -1 - i));
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setVisible(false);
                bottomLayer->setVisible(false);
            }
            stackPosition += 32;
            break;
        case DesignLayer::MechanicalCategory:
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(layerName(category, i));
                bottomLayer->setName(layerName(category, 32 - 1 - i));
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setVisible(false);
                bottomLayer->setVisible(false);
            }
            stackPosition += 32;
            break;
        case DesignLayer::OtherCategory:
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 0));
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 1));
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 2));
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 3));
            break;
        case DesignLayer::MaskCategory:
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 0));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(layerName(category, 1));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 2));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(layerName(category, 3));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
            break;
        case DesignLayer::SilkscreenCategory:
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(layerName(category, 0));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(layerName(category, 1));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setVisible(true);
            bottomLayer->setVisible(true);
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

DesignLayerList DesignLayerManager::layersForCategory(DesignLayer::Category category) const
{
    Q_ASSERT(m_layerCategoryMap.contains(category));
    DesignLayerList list = m_layerCategoryMap[category];
    qSort(list.begin(), list.end(),
          [](DesignLayer *first, DesignLayer *second) {
       return first->stackPosition() < second->stackPosition();
    });
    return list;
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

QString DesignLayerManager::layerName(DesignLayer::Category category, int categoryIndex)
{
    switch (category) {
    case DesignLayer::SignalCategory:
        switch (categoryIndex) {
        case 0:
            return QString("Top Layer");
        case 31:
            return QString("Bottom Layer");
        default:
            return QString("Signal Layer %1").arg(categoryIndex);
        }
    case DesignLayer::PlaneCategory:
        return QString("Internal Plane %1").arg(categoryIndex + 1);
    case DesignLayer::MaskCategory:
        switch (categoryIndex) {
        case 0:
            return QString("Top Paste");
        case 1:
            return QString("Bottom Paste");
        case 2:
            return QString("Top Solder");
        case 3:
            return QString("Bottom Solder");
        default:
            return QString("Invalid Mask Layer!");
        }
    case DesignLayer::SilkscreenCategory:
        switch (categoryIndex) {
        case 0:
            return QString("Top Overlay");
        case 1:
            return QString("Bottom Overlay");
        default:
            return QString("Invalid Silkscreen Layer!");
        }
    case DesignLayer::MechanicalCategory:
        return QString("Mechanical %1").arg(categoryIndex + 1);
    case DesignLayer::OtherCategory:
        switch (categoryIndex) {
        case 0:
            return QString("Drill Guide");
        case 1:
            return QString("Keep Out");
        case 2:
            return QString("Drill Drawing");
        case 3:
            return QString("Multi Layer");
        default:
            return QString("Invalid Other Layer!");
        }
    default:
        return QString("Invalid Layer!");
    }
}

QString DesignLayerManager::categoryName(DesignLayer::Category category)
{
    switch (category) {
    case DesignLayer::SignalCategory:
         return QString("Signal Layers");
    case DesignLayer::PlaneCategory:
        return QString("Internal Planes");
    case DesignLayer::MaskCategory:
        return QString("Mask Layers");
    case DesignLayer::SilkscreenCategory:
        return QString("Silkscreen Layers");
    case DesignLayer::MechanicalCategory:
        return QString("Mechanical Layers");
    case DesignLayer::OtherCategory:
        return QString("Other Layers");
    default:
        return QString("Invalid Layer!");
    }
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
DesignLayer *DesignLayerManager::addLayer(DesignLayer::Category category, int stackPosition)
{
    Q_ASSERT(!m_layerMap.contains(stackPosition));
    Q_ASSERT(m_layerCategoryMap.contains(int(category)));
    DesignLayer *layer = new DesignLayer();
    layer->setCategory(category);
    layer->setStackPosition(stackPosition);
    m_layerMap[stackPosition] = layer;
    m_layerCategoryMap[int(category)].append(layer);
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

