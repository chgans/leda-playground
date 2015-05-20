#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QMap>
#include <QVariant>

namespace architecture
{

struct Component
{
    Component(const QString &name = QString(),
              const QString &entityName = QString(),
              const QString &entityArchitectureName = QString()):
        name(name),
        entityName(entityName),
        entityArchitectureName(entityArchitectureName)
    {}

    QString name;
    QString entityName;
    QString entityArchitectureName;
    QMap<QString, QString> portMap;
    // TODO: QString: allow for unit, unit prefix and parametric-ish definition
    QMap<QString, QVariant> genericMap;
};

}
#endif // COMPONENT_H
