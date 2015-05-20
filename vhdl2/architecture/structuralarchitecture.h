#ifndef STRUCTURALARCHITECTURE_H
#define STRUCTURALARCHITECTURE_H

#include "architecture.h"
#include "signal.h"
#include "component.h"

namespace architecture
{

struct StructuralArchitecture : public Architecture
{
    StructuralArchitecture(const QString &name = QString(),
                           const QString &entityName = QString()):
        Architecture(name, entityName)
    {}

    QList<Signal *> signalList;
    QList<Component *> componentList;
    QMap<QString, QString> portMap;
};

}
#endif // STRUCTURALARCHITECTURE_H
