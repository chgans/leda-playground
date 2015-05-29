#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include <QList>
#include <QPixmap>

class QGraphicsItem;
class SymbolCollection;

class Symbol
{
public:
    Symbol(SymbolCollection *collection = 0);

    SymbolCollection *collection() const
    {
        return m_collection;
    }
    void setCollection(SymbolCollection *collection)
    {
        m_collection = collection;
    }

    QString caption;
    QString description;
    QList<QGraphicsItem*> drawings;
    QPixmap pixmap() const;

private:
    SymbolCollection *m_collection;
    mutable QPixmap m_pixmap;
};

class SymbolCollection
{
public:
    SymbolCollection();
    ~SymbolCollection();

    QString name;
    QList<Symbol *> symbols;
};

#endif // SYMBOL_H
