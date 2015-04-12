#ifndef DESIGNLAYERMANAGER_H
#define DESIGNLAYERMANAGER_H

#include <QObject>

#include "designlayer.h"

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
    int customLayerSetCount() const;
    DesignLayerList layerSet(DesignLayer::LayerSet set) const;
    DesignLayerList customLayerSet(int id) const;
    DesignLayerList allLayers() const;
    DesignLayerList layersForCategory(DesignLayer::Category category) const;
    DesignLayer * layerAt(int stackPosition) const;
    void setLayerEnabled(int stackPosition, bool enabled);
    bool isLayerEnabled(int stackPosition) const;
    DesignLayerList enabledLayers() const;
    static QString layerName(DesignLayer::Category category, int categoryIndex);
    static QString categoryName(DesignLayer::Category category);

signals:
    void layerAdded(DesignLayer *layer);
    void layerRemoved(DesignLayer *layer);
    void customLayerSetAdded(int id);
    void customLayerSetRemoved(int id);
    void layerEnabledChanged(int stackPosition, bool enabled);

public slots:
    DesignLayerList addCustomLayerSet(int id, const QString &name);
    void removeCustomLayerSet(int id);
    DesignLayer *addLayer(DesignLayer::Category category, int stackPosition);
    void removeLayer(DesignLayer *layer);

private:
    static DesignLayerManager *m_instance;
    QMap<int, DesignLayer*> m_layerMap;
    QMap<int, DesignLayerList> m_layerCategoryMap;
    QMap<int, DesignLayerList> m_layerSetMap;
    QMap<int, DesignLayerList> m_customLayerSetMap;
};


#endif // DESIGNLAYERMANAGER_H
