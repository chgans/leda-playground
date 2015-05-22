#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <QString>
#include <QMap>
#include <QList>


// Configuration in library:
//  - simu model for a manufacturer series of diode, MOSFETs, ...
//   -> For D1: diode use configuration com.diodeincorporated.models.BAT54WS

// TODO: recursive block config

struct ArchitectureConfiguration;
typedef QList<ArchitectureConfiguration *> ArchitectureConfigurationList;
struct ComponentConfiguration;
typedef QList<ComponentConfiguration *> ComponentConfigurationList;

// configuration configuration_name of entity_name is
struct Configuration
{
    Configuration(const QString &name, const QString &entityName):
        name(name), entityName(entityName)
    {}

    QString name;
    QString entityName;

    ArchitectureConfigurationList archConfigList;
    // QucsArch, SchematicsArch, LayoutArch
};

// for architecture_name
struct ArchitectureConfiguration
{
    // TBD: ArchConfig<QucsArchitecture>(QucsArchitecture *arch)
    ArchitectureConfiguration(const QString &archName):
        architectureName(archName)
    {}

    QString architectureName;
    // TBD: generic and port maps?
    ComponentConfigurationList compConfigList;
};

// for instance_label:component_name
//  use entity library_name.entity_name(arch_name);
// endfor
// for instance_label:component_name
//  use configuration library_name.config_name;
// endfor
// for instance_label:component_name
//  -- defaults
// endfor
struct ComponentConfiguration
{
    ComponentConfiguration(const QString &instanceLabel, const QString &componentName,
                           const QString &libName):
        instanceLabel(instanceLabel), componentName(componentName),
        libName(libName)
    {}

    QString instanceLabel; // Can be wildcard
    QString componentName;

    QString libName;

    QString entityName;
    QString architectureName;

    QString configurationName;

    QMap<QString, QString> genericMap;
    QMap<QString, QString> portMap;
};

#endif // CONFIGURATION_H
