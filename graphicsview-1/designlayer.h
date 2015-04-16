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

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int stackPosition READ stackPosition WRITE setStackPosition NOTIFY stackPositionChanged)
    Q_PROPERTY(Category category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(Face face READ face WRITE setFace NOTIFY faceChanged)
    Q_PROPERTY(DesignLayer* pairedLayer READ pairedLayer WRITE setPairedLayer NOTIFY pairedLayerChanged)

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

    QString name() const;
    QString customName() const;
    QString effectiveName() const;
    const QColor &color() const;
    int stackPosition() const; // TODO: Rename to index()
    Category category() const;
    Face face() const;
    DesignLayer* pairedLayer() const;
    bool isValid() const;
    bool isUsed() const;
    qreal opacityForPrimitive(Primitive::Type type);
    void setOpacityForPrimitive(Primitive::Type type, qreal opacity);

signals:
    void nameChanged(const QString &name);
    void customNameChanged(const QString &name);
    void colorChanged(const QColor &color);
    void stackPositionChanged(int position);
    void categoryChanged(Category category);
    void faceChanged(Face arg);
    void pairedLayerChanged(DesignLayer* pairedLayer);

public slots:
    void setName(const QString &name);
    void setCustomName(const QString &name);
    void setColor(const QColor &color);
    void setStackPosition(int position);
    void setCategory(Category category);
    void setFace(Face arg);
    void setPairedLayer(DesignLayer* layer);

private:
    QString m_name;
    QString m_customName;
    QColor m_color;
    int m_stackPosition;
    Category m_category;
    Face m_face;
    DesignLayer* m_pairedLayer;
    QMap<Primitive::Type, qreal> m_primitiveOpacityMap;

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
