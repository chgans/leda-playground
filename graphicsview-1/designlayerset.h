#ifndef DESIGNLAYERSET_H
#define DESIGNLAYERSET_H

#include <QObject>
#include <QList>
#include <QString>

class DesignLayer;
class DesignLayerManager;

class DesignLayerSet: public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString customName READ customName WRITE setCustomName NOTIFY customNameChanged)
    Q_PROPERTY(QString effectiveName READ effectiveName NOTIFY effectiveNameChanged STORED false)
public:
    enum Type {
        Signal = 0,
        Plane,
        Mask,
        Silkscreen,
        Mechanical,
        NonSignal,
        Custom = 1000
    };

    DesignLayerSet(QObject *parent = 0);
    ~DesignLayerSet();

    QString name() const;
    QString customName() const;
    QString effectiveName() const;
    bool isBuiltIn() const;
    int type() const;

    bool contains(DesignLayer *layer) const;

    QList<DesignLayer *> allLayers();
    QList<DesignLayer *> enabledLayers();
    int layerCount();

signals:
    void nameChanged(const QString &name);
    void customNameChanged(const QString &name);
    void effectiveNameChanged(const QString &name);

private:
    friend class DesignLayerManager;
    QList<DesignLayer *> m_layers;
    int m_type;
    QString m_name;
    QString m_customName;
    bool m_builtIn;

    void add(DesignLayer *layer);
    void remove(DesignLayer *layer);
    void setName(const QString &name);
    void setCustomName(const QString &name);
    void setBuiltIn(bool builtIn);
    void setType(int type);
};

//Q_DECLARE_METATYPE(DesignLayerSet::Type)

#endif // DESIGNLAYERSET_H
