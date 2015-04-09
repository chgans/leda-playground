#include "designlayer.h"

DesignLayer::DesignLayer(QGraphicsItem *parent):
    QGraphicsItem(parent)
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
    m_name = name;
}

const QColor &DesignLayer::color() const
{
    return m_color;
}

void DesignLayer::setColor(const QColor &color)
{
    prepareGeometryChange();
    m_color = color;
    update();
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

