#ifndef ENTITY_H
#define ENTITY_H

#include <QString>;
#include <QList>
#include <QVariant>

/* VHDL user attribute
 *
 * Declaration:
 * attribute attribute_name: type;
 * Specification:
 * attribute attribute_name of item : item_class is expression;
 */

class EdlPort;
class EdlSignal;
class EdlAttribute;

class EdlEntity
{
public:
    EdlEntity() {}
    QString id;
    QList<EdlPort> portList;
    QList<EdlAttribute> attributeList;
};

class EdlPort
{
public:
    enum ElectricalType {
        InPort,
        OutPort,
        InOutPort, // HiZ, Passive, Power, ...
    };

    EdlPort() {}
    QString id;
    QString label;
    ElectricalType direction;
    // TODO: SignalType
};

class EdlAttribute
{
public:
    EdlAttribute() {}
    QString id;
    QMetaType::Type dataType();
    QVariant defaultValue;
};

// ----

class EdlSignal
{
public:
    EdlSignal() {}
    QString Id;
    int SignalTypeId; // single, pair, series or aggregation
};

class EdlInstance
{
public:
    EdlInstance() {}
    QString instanceId;
    QString entityIdRef; // can be infered from architecture
    QString architectureIdRef;
    // TBD: This or mapping within architecture: local signal <=> <instanceId, EntityPortId>
    QMap<QString, QString> portMapping; // local signal <=> ref entity's port
};

class EdlArchitecture
{
public:
    EdlArchitecture() {}
    QString id;
    QString entityIdRef;
    QList<EdlSignal> signalList;
    QList<EdlAttribute> attributeList;
    QList<EdlInstance> instanceList;
    QMap<QString, QString> portMapping; // local signal <=> local entity's port
};



// Implementation: Signal integrity, simulation, schematics, layout
#endif // ENTITY_H
