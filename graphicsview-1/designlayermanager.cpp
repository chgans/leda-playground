#include "designlayermanager.h"
#include "designlayerset.h"

#include <QDebug>

// FIXME: use SOFT_ASSERT in some places to generate warning/error messages

/*
 * TODO: allow user to modify any layer or sets, while still being a central
 *       place for notifications.
 *  - use properties with signal/slots on DesignLayer and DesignLayerSet
 *  - monitor all layers ans all sets, and proxy-emit the property changes
 */

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
    QList<DesignLayer *> layers;

    addBuiltInLayerSet(DesignLayerSet::All);
    addBuiltInLayerSet(DesignLayerSet::Signal);
    addBuiltInLayerSet(DesignLayerSet::Plane);
    addBuiltInLayerSet(DesignLayerSet::Mechanical);
    addBuiltInLayerSet(DesignLayerSet::Mask);
    addBuiltInLayerSet(DesignLayerSet::Silkscreen);
    addBuiltInLayerSet(DesignLayerSet::NonSignal);

    QList<DesignLayer::Category> categories;
    categories << DesignLayer::SignalCategory
               << DesignLayer::PlaneCategory
               << DesignLayer::MechanicalCategory
               << DesignLayer::MaskCategory
               << DesignLayer::OtherCategory
               << DesignLayer::SilkscreenCategory;

    foreach (DesignLayer::Category category, categories) {

        m_layerCategoryMap[category] = DesignLayerList();

        switch (category) {
        case DesignLayer::SignalCategory:
            layers.clear();
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(defaultLayerName(category, i));
                bottomLayer->setName(defaultLayerName(category, 32 - 1 - i));
                if (i == 0) {
                    topLayer->setPresent(true);
                    bottomLayer->setPresent(true);
                }
                else {
                    topLayer->setPresent(false);
                    bottomLayer->setPresent(false);
                }
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                layers << topLayer << bottomLayer;
            }
            layerSet(DesignLayerSet::Signal)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
            stackPosition += 32;
            break;
        case DesignLayer::PlaneCategory:
            layers.clear();
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(defaultLayerName(category, i));
                bottomLayer->setName(defaultLayerName(category, 32 -1 - i));
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setPresent(false);
                bottomLayer->setPresent(false);
                layers << topLayer << bottomLayer;
            }
            layerSet(DesignLayerSet::Plane)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
            stackPosition += 32;
            break;
        case DesignLayer::MechanicalCategory:
            layers.clear();
            for (int i = 0; i < 16; i++) {
                topLayer = addLayer(category, stackPosition + i);
                bottomLayer = addLayer(category, stackPosition + 32 - 1 - i);
                topLayer->setName(defaultLayerName(category, i));
                bottomLayer->setName(defaultLayerName(category, 32 - 1 - i));
                topLayer->setPairedLayer(bottomLayer);
                bottomLayer->setPairedLayer(topLayer);
                topLayer->setFace(DesignLayer::TopFace);
                bottomLayer->setFace(DesignLayer::BottomFace);
                topLayer->setPresent(false);
                bottomLayer->setPresent(false);
                layers << topLayer << bottomLayer;
            }
            layerSet(DesignLayerSet::Mechanical)->add(layers);
            layerSet(DesignLayerSet::NonSignal)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
            stackPosition += 32;
            break;
        case DesignLayer::OtherCategory:
            layers.clear();
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 0));
            layers << topLayer;
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 1));
            layers << topLayer;
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 2));
            layers << topLayer;
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 3));
            layers << topLayer;
            layerSet(DesignLayerSet::NonSignal)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
            break;
        case DesignLayer::MaskCategory:
            layers.clear();
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 0));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(defaultLayerName(category, 1));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setPresent(true);
            bottomLayer->setPresent(true);
            layers << topLayer << bottomLayer;
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 2));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(defaultLayerName(category, 3));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setPresent(true);
            bottomLayer->setPresent(true);
            layers << topLayer << bottomLayer;
            layerSet(DesignLayerSet::Mask)->add(layers);
            layerSet(DesignLayerSet::NonSignal)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
            break;
        case DesignLayer::SilkscreenCategory:
            layers.clear();
            topLayer = addLayer(category, stackPosition++);
            topLayer->setName(defaultLayerName(category, 0));
            bottomLayer = addLayer(category, stackPosition++);
            bottomLayer->setName(defaultLayerName(category, 1));
            topLayer->setPairedLayer(bottomLayer);
            bottomLayer->setPairedLayer(topLayer);
            topLayer->setFace(DesignLayer::TopFace);
            bottomLayer->setFace(DesignLayer::BottomFace);
            topLayer->setPresent(true);
            bottomLayer->setPresent(true);
            layers << topLayer << bottomLayer;
            layerSet(DesignLayerSet::Silkscreen)->add(layers);
            layerSet(DesignLayerSet::NonSignal)->add(layers);
            layerSet(DesignLayerSet::All)->add(layers);
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

QList<DesignLayerSet *> DesignLayerManager::allLayerSets() const
{
    return m_layerSetMap.values();
}

DesignLayerSet *DesignLayerManager::layerSet(int type) const
{
    Q_ASSERT(m_layerSetMap.contains(int(type)));
    return m_layerSetMap[int(type)];
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
    qDebug() << __PRETTY_FUNCTION__ << stackPosition << enabled;
    Q_ASSERT(m_layerMap.contains(stackPosition));
    if (m_layerMap[stackPosition]->isPresent() == enabled)
        return;
    m_layerMap[stackPosition]->setPresent(enabled);
    emit layerChanged(m_layerMap[stackPosition]);
}

bool DesignLayerManager::isLayerEnabled(int stackPosition) const
{
    Q_ASSERT(m_layerMap.contains(stackPosition));
    return m_layerMap[stackPosition]->isPresent();
}

DesignLayerList DesignLayerManager::enabledLayers() const
{
    DesignLayerList list;
    foreach (DesignLayer *layer, m_layerMap.values()) {
        if (layer->isPresent())
            list.append(layer);
    }
    return list;
}

void DesignLayerManager::enableOnlyUsedLayers()
{
    foreach (DesignLayer *layer, m_layerMap.values()) {
        setLayerEnabled(layer->stackPosition(), layer->isUsed());
    }
}

DesignLayerList DesignLayerManager::usedLayers() const
{
    DesignLayerList list;
    foreach (DesignLayer *layer, m_layerMap.values()) {
        if (layer->isUsed())
            list.append(layer);
    }
    return list;
}

QString DesignLayerManager::defaultLayerName(DesignLayer::Category category, int categoryIndex)
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

QString DesignLayerManager::builtInLayerSetName(int type)
{
    switch (type) {
    case DesignLayerSet::All:
        return QString("All layers");
    case DesignLayerSet::Signal:
        return QString("Signal layers");
    case DesignLayerSet::Plane:
        return QString("Plane layers");
    case DesignLayerSet::Mask:
        return QString("Mask layers");
    case DesignLayerSet::Silkscreen:
        return QString("Silkscreen layers");
    case DesignLayerSet::Mechanical:
        return QString("Mechnical layers");
    case DesignLayerSet::NonSignal:
        return QString("Non Signal layers");
    case DesignLayerSet::Custom:
        return QString("Custom layer set");
    }
    return QString("Invalid layer set!");
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

DesignLayerSet *DesignLayerManager::addLayerSet(int id)
{
    Q_ASSERT(!m_layerSetMap.contains(id));
    DesignLayerSet *set = new DesignLayerSet();
    set->setType(id);
    m_layerSetMap[id] = set;
    return m_layerSetMap[id];
}

DesignLayerSet *DesignLayerManager::addBuiltInLayerSet(int type)
{
    DesignLayerSet *set = addLayerSet(type);
    set->setBuiltIn(true);
    set->setName(builtInLayerSetName(type));
    set->setType(type);
    emit layerSetAdded(set);
    return set;
}

void DesignLayerManager::removeLayerSet(int id)
{
    // FIXME: use SOFT_ASSERT to generate warning/error messages
    Q_ASSERT(m_layerSetMap.contains(id));
    m_layerSetMap.remove(id);
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
    m_layerMap.remove(layer->stackPosition());
    foreach (DesignLayerSet *set, m_layerSetMap.values()) {
        if (set->contains(layer))
            set->remove(layer);
    }
}

void DesignLayerManager::addLayerToSet(DesignLayer *layer, DesignLayerSet *set)
{
    set->add(layer);
}

