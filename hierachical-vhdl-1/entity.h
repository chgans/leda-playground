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


class BaseSignalSpec
{
public:
    virtual ~BaseSignalSpec();

    enum SignalSpecType {
        Base = 0,
        Single,
        Array,
        Aggregate
    };

    QString name() const;
    void setName(const QString &name);

    virtual int type() const;

protected:
    explicit BaseSignalSpec(const QString &name);
    BaseSignalSpec(const BaseSignalSpec &other);

private:
    QString m_name;
};

class SingleSignalSpec: public BaseSignalSpec
{
public:
    explicit SingleSignalSpec(const QString &name = QString());
    SingleSignalSpec(const SingleSignalSpec &other);
    ~SingleSignalSpec();

    int type() const;
};


class ArraySignalSpec: public BaseSignalSpec
{
public:
    explicit ArraySignalSpec(const QString &name = QString(),
                             int size = 0);
    ArraySignalSpec(const ArraySignalSpec &other);

    ~ArraySignalSpec();

    void setSize(int size);
    int size() const;

private:
    int m_size;
};

class AggregateSignalSpec: public BaseSignalSpec
{
public:
    explicit AggregateSignalSpec(const QString &name = QString());
    AggregateSignalSpec(const AggregateSignalSpec &other);
    ~AggregateSignalSpec();

    int size() const;
    const BaseSignalSpec *signal(int index) const;
    const BaseSignalSpec *signal(const QString &name) const;
    bool addSignal(BaseSignalSpec *spec);
    bool removeSignal(int index);
    bool removeSignal(const QString &name);

private:
    QList<BaseSignalSpec*> m_signals;
    QMap<QString, BaseSignalSpec*> m_signalMap;
};

/*
class PortSpec
{
public:
    explicit PortSpec(const QString &name = QString(),
                               PortType type = PassivePort,
                               BaseSignalSpec *spec = 0);

    QString name() const;
    void setName(const QString &name);

    PortType portType() const;
    void setPortType(PortType type);

    const BaseSignalSpec &signalSpec() const;
    void setSignalSpec(const BaseSignalSpec &spec);

private:
    QString m_name;
    PortType m_portType;
    BaseSignalSpec m_signalSpec;
};
typedef QMap<QString, PortSpec*> PortSpecMap;

class Entity
{
public:
    explicit Entity();
    ~Entity();

private:
    QString m_name;
    QVariantMap m_generics;
    QVariantMap m_constants;
    QList<PortSpec*> m_ports;
};

class Package
{
private:
    QString m_name;
    QVariantMap m_generics;

};
*/
#endif // ENTITY_H
