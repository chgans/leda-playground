#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QString>

#include "component.h"
#include "signal.h"

namespace architecture
{

struct Architecture
{
    Architecture(const QString &name = QString(),
                 const QString &entityName = QString()):
        name(name),
        entityName(entityName)
    {}

    QString name;
    QString entityName;
};

}
#endif // ARCHITECTURE_H
