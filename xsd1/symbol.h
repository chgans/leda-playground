#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include <QList>

class QGraphicsItem;

class Symbol
{
public:
    QString name;
    QString label;
    QList<QGraphicsItem*> drawings;
};

#endif // SYMBOL_H
