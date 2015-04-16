#include "designlayer.h"

/*
 * TODO:
 *  - DesignLayer is just a QObject
 *  - DesignLayer has a QGraphicsItem based laterItem
 *  - rename present to enable
 */

DesignLayer::DesignLayer(QGraphicsItem *parent):
    QGraphicsObject(parent),
    m_stackPosition(-1),
    m_category(InvalidCategory),
    m_face(InvalidFace),
    m_pairedLayer(nullptr)
{
    setFlags(ItemHasNoContents);
    for (Primitive::Type type = Primitive::_BeginType; type < Primitive::_EndType; type = Primitive::Type(type + 1)) {
        m_primitiveOpacityMap[type] = 1.0;
    }
}

DesignLayer::~DesignLayer()
{

}

QString DesignLayer::name() const
{
    return m_name;
}

QString DesignLayer::customName() const
{
    return m_customName;
}

QString DesignLayer::effectiveName() const
{
    if (m_customName.isEmpty())
        return m_name;
    return m_customName;
}

void DesignLayer::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;

    emit nameChanged(m_name);
}

void DesignLayer::setCustomName(const QString &name)
{
    if (m_customName == name)
        return;

    m_customName = name;

    emit customNameChanged(m_customName);
}

const QColor &DesignLayer::color() const
{
    return m_color;
}

void DesignLayer::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;

    emit colorChanged(m_color);
}

QRectF DesignLayer::boundingRect() const
{
    return QRectF();
}

void DesignLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

DesignLayer *DesignLayer::pairedLayer() const
{
    return m_pairedLayer;
}

bool DesignLayer::isValid() const
{
    return !m_name.isEmpty() && m_color.isValid() && m_stackPosition != -1 &&
            m_category != InvalidCategory && m_face != InvalidFace;
}

bool DesignLayer::isUsed() const
{
    return childItems().count() != 0;
}

qreal DesignLayer::opacityForPrimitive(Primitive::Type type)
{
    Q_ASSERT(m_primitiveOpacityMap.contains(type));
    return m_primitiveOpacityMap[type];
}

void DesignLayer::setOpacityForPrimitive(Primitive::Type type, qreal opacity)
{
    Q_ASSERT(m_primitiveOpacityMap.contains(type));
    m_primitiveOpacityMap[type] = opacity;
}

bool DesignLayer::isPresent() const
{
    return m_present;
}

DesignLayer::Face DesignLayer::face() const
{
    return m_face;
}

int DesignLayer::stackPosition() const
{
    return m_stackPosition;
}

DesignLayer::Category DesignLayer::category() const
{
    return m_category;
}

void DesignLayer::setStackPosition(int position)
{
    if (m_stackPosition == position)
        return;

    m_stackPosition = position;
    emit stackPositionChanged(position);
}

void DesignLayer::setCategory(DesignLayer::Category category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged(category);
}

void DesignLayer::setFace(DesignLayer::Face arg)
{
    if (m_face == arg)
        return;

    m_face = arg;
    emit faceChanged(arg);
}

void DesignLayer::setPairedLayer(DesignLayer *layer)
{
    if (m_pairedLayer == layer)
        return;

    m_pairedLayer = layer;
    emit pairedLayerChanged(layer);
}

void DesignLayer::setPresent(bool arg)
{
    if (m_present == arg)
        return;

    m_present = arg;
    emit presentChanged(arg);
}
