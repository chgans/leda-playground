#ifndef DESIGNLAYERMANAGER_H
#define DESIGNLAYERMANAGER_H

#include <QObject>

#include "designlayer.h"
#include "designlayerset.h"

class QSettings;

class DesignLayerManager : public QObject
{
    Q_OBJECT

private:
    explicit DesignLayerManager(QObject *parent = 0);

public:    
    ~DesignLayerManager();

    static DesignLayerManager *instance();
    void loadFromSettings(const QSettings &settings);
    void saveToSettings(QSettings *settings) const;
    void loadFromDefaults();
    int layerCount() const;
    int layerSetCount() const;
    QList<DesignLayerSet *> allLayerSets() const;
    DesignLayerSet *layerSet(int type) const;
    DesignLayerList allLayers() const;
    DesignLayerList layersForCategory(DesignLayer::Category category) const;
    DesignLayer *layerAt(int stackPosition) const;
    void setLayerEnabled(int stackPosition, bool enabled);
    bool isLayerEnabled(int stackPosition) const;
    DesignLayerList enabledLayers() const;

    static QString defaultLayerName(DesignLayer::Category category, int categoryIndex);
    static QString builtInLayerSetName(int type);
    static QString categoryName(DesignLayer::Category category);

signals:
    void layerAdded(DesignLayer *layer);
    void layerRemoved(DesignLayer *layer);
    void layerChanged(DesignLayer *layer);
    void layerSetAdded(DesignLayerSet *set);
    void layerSetRemoved(DesignLayerSet *set);
    void layerSetChanged(DesignLayerSet *set);

private:
    DesignLayerSet *addLayerSet(int type);
    DesignLayerSet *addBuiltInLayerSet(int type);
    void removeLayerSet(int type);
    DesignLayer *addLayer(DesignLayer::Category category, int stackPosition);
    void removeLayer(DesignLayer *layer);
    void addLayerToSet(DesignLayer *layer, DesignLayerSet *set);

private:
    static DesignLayerManager *m_instance;
    QMap<int, DesignLayer*> m_layerMap;
    QMap<int, DesignLayerList> m_layerCategoryMap;
    QMap<int, DesignLayerSet*> m_layerSetMap;
};


#endif // DESIGNLAYERMANAGER_H
