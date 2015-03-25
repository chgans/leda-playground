#include "scenelayer.h"

GSceneLayer::GSceneLayer(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    setFlags(ItemHasNoContents);
}

GSceneLayer::~GSceneLayer()
{

}

QString GSceneLayer::name() const
{
    return m_name;
}

void GSceneLayer::setName(const QString &name)
{
    m_name = name;
}

const QColor &GSceneLayer::color() const
{
    return m_color;
}

void GSceneLayer::setColor(const QColor &color)
{
    prepareGeometryChange();
    m_color = color;
    update();
}

QRectF GSceneLayer::boundingRect() const
{
    return QRectF();
}

void GSceneLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

