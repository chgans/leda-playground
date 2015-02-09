#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QVariantMap>

/*
 * VHDL:
 *  bit, bit vector,
 *  bool, bool vector,
 *  integer, int vector,
 *  natural, positive,
 *  char, string, time (duration)
 *  array of
 *  custom using range, array, record
 * Gnu radio:
 *  complex, float, int, long, byte, string
 *  vector of
 * Altium Sch:
 *  signal, bus, harness
*/

enum PortType
{
    InputPort,
    OutputPort,
    InputOutputPort,
    PassivePort
};

class SignalSpecification
{
public:
    explicit SignalSpecification() {}

private:
    QString m_name;
};

class SingleSignalSpecification: public SignalSpecification
{

};

class ArraySignalSpecification: public SignalSpecification
{
private:
    int m_size;
};

class AggregateSignalSpecification: public SignalSpecification
{
private:
    QList<SignalSpecification> m_signals;
};


class PortSpecification
{
public:
    explicit PortSpec();
private:
    QString m_name;
    PortType m_portType;
    SignalSpecification m_signalSpec;
};


class Entity
{
public:
    explicit Entity();
    ~Entity();

private:
    QString m_name;
    QVariantMap m_generics;
    QVariantMap m_constants;
    QList<PortSpecification> m_ports;
};

#endif // ENTITY_H
