#include "designlayer.h"

DesignLayer::DesignLayer(QGraphicsItem *parent):
    QGraphicsObject(parent)
{
    setFlags(ItemHasNoContents);
}

DesignLayer::~DesignLayer()
{

}

QString DesignLayer::name() const
{
    return m_name;
}

void DesignLayer::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;

    emit nameChanged(m_name);
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

