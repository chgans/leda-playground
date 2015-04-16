#ifndef SCENELAYER_H
#define SCENELAYER_H

#include <QGraphicsObject>
#include <QMap>
#include <QString>

#include "primitive.h"

class DesignLayerManager;

class DesignLayer : public QGraphicsObject
{
    Q_OBJECT

    Q_ENUMS(Category)
    Q_ENUMS(Face)

    Q_PROPERTY(QString defaultName READ defaultName WRITE setDefaultName NOTIFY defaultNameChanged)
    Q_PROPERTY(QString customName READ customName WRITE setCustomName NOTIFY customNameChanged)
    Q_PROPERTY(QString effectiveName READ effectiveName NOTIFY effectiveNameChanged STORED false)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int index READ index WRITE setStackPosition NOTIFY stackPositionChanged)
    Q_PROPERTY(Category category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(Face face READ face WRITE setFace NOTIFY faceChanged)
    Q_PROPERTY(DesignLayer* pairedLayer READ pairedLayer WRITE setPairedLayer NOTIFY pairedLayerChanged)
    Q_PROPERTY(bool present READ isPresent WRITE setPresent NOTIFY presentChanged)
public:
    enum Face {
        InvalidFace = 0,
        TopFace,
        BottomFace
    };

    enum Category {
        InvalidCategory = 0,
        SignalCategory,
        PlaneCategory,
        MechanicalCategory,
        MaskCategory,
        OtherCategory,
        SilkscreenCategory
    };

    ~DesignLayer();

    QString defaultName() const;
    QString customName() const;
    QString effectiveName() const;
    const QColor &color() const;
    int index() const; // TODO: Rename to index()
    Category category() const;
    Face face() const;
    DesignLayer* pairedLayer() const;
    bool isValid() const;
    bool isUsed() const;
    qreal opacityForPrimitive(Primitive::Type type);
    void setOpacityForPrimitive(Primitive::Type type, qreal opacity);
    bool isPresent() const;

signals:
    void defaultNameChanged(const QString &defaultName);
    void customNameChanged(const QString &defaultName);
    void effectiveNameChanged(const QString &defaultName);
    void colorChanged(const QColor &color);
    void stackPositionChanged(int position);
    void categoryChanged(Category category);
    void faceChanged(Face arg);
    void pairedLayerChanged(DesignLayer* pairedLayer);
    void presentChanged(bool arg);

public slots:
    void setDefaultName(const QString &defaultName);
    void setCustomName(const QString &defaultName);
    void setColor(const QColor &color);
    void setStackPosition(int position);
    void setCategory(Category category);
    void setFace(Face arg);
    void setPairedLayer(DesignLayer* layer);
    void setPresent(bool arg);

private:
    QString m_name;
    QString m_customName;
    QColor m_color;
    int m_index;
    Category m_category;
    Face m_face;
    DesignLayer* m_pairedLayer;
    QMap<Primitive::Type, qreal> m_primitiveOpacityMap;
    bool m_present;

protected:
    DesignLayer(QGraphicsItem *parent = 0);
    friend class DesignLayerManager;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

Q_DECLARE_METATYPE(DesignLayer::Category)
Q_DECLARE_METATYPE(DesignLayer::Face)
Q_DECLARE_METATYPE(DesignLayer*)

typedef QList<DesignLayer *> DesignLayerList;

#endif // SCENELAYER_H
