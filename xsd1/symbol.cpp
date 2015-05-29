#include "symbol.h"

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPixmap>
#include <QPainter>

Symbol::Symbol(SymbolCollection *collection):
    m_collection(collection)
{

}

QPixmap Symbol::pixmap() const
{
    if (!m_pixmap.isNull())
        return m_pixmap;

    QGraphicsScene scene;
    QGraphicsItemGroup *group = scene.createItemGroup(drawings);
    QSize size = group->boundingRect().size().toSize();
    m_pixmap = QPixmap(size);
    QPainter painter;
    painter.begin(&m_pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    painter.end();
    return m_pixmap;
}


SymbolCollection::SymbolCollection()
{

}

SymbolCollection::~SymbolCollection()
{
    qDeleteAll(symbols);
}
