#ifndef SCENELAYER_H
#define SCENELAYER_H

#include <QGraphicsObject>

class DesignLayer : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
#if 0
    enum LayerType {
        NoLayerType = 0,
        MultiLayerLayerType,
        SolderPasteMaskLayerType,
        SolderResistMaskLayerType,
        GlueDotMaskLayerType,
        SilkScreenLayerType,
        SignalLayerType,
        PlaneLayerType,
        DrillGuideLayerType,
        DrillDrawingLayerType,
        KeepOutLayerType,
        MechanicalLayerType,
    };

    enum LayerGroup {
        NoLayerGroup = 0,
        SignalLayerGroup,
        PlaneLayerGroup,
        MechanicalLayerGroup,
        OtherLayerGroup,
        WorkspaceLayerGroup,
        SystemLayerGroup
    };

    SceneLayer(QGraphicsItem *parent = 0);
    ~SceneLayer();

    LayerType layerType() const;
    void setLayerType(LayerType type);
    LayerGroup layerGroup() const;
    void setLayerGroup(LayerGroup group); // In scene?

    SceneLayer *pairedLayer() const;
    void setPairedLayer(SceneLayer *layer); // Typ. top/bottom

    QString shortName() const;
    QString mediumName() const;
    QString longName() const;
    void setNames(const QString &names);
#endif

    DesignLayer(QGraphicsItem *parent = 0);
    ~DesignLayer();

    QString name() const;
    void setName(const QString &name);

    const QColor &color() const;
    void setColor(const QColor &color);

signals:
    void nameChanged(const QString &name);
    void colorChanged(const QColor &color);

private:
    QString m_name;
    QColor m_color;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SCENELAYER_H
