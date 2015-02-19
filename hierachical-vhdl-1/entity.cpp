#include "entity.h"

BaseSignalSpec::~BaseSignalSpec()
{

}

QString BaseSignalSpec::name() const
{
    return m_name;
}

void BaseSignalSpec::setName(const QString &name)
{
    m_name = name;
}

int BaseSignalSpec::type() const
{
    return Base;
}

BaseSignalSpec::BaseSignalSpec(const QString &name):
    m_name(name)
{

}

BaseSignalSpec::BaseSignalSpec(const BaseSignalSpec &other):
    m_name(other.m_name)
{

}


SingleSignalSpec::SingleSignalSpec(const QString &name):
    BaseSignalSpec(name)
{

}

SingleSignalSpec::SingleSignalSpec(const SingleSignalSpec &other):
    BaseSignalSpec(other)
{

}

SingleSignalSpec::~SingleSignalSpec()
{

}

int SingleSignalSpec::type() const
{
  return Single;
}



ArraySignalSpec::ArraySignalSpec(const QString &name, int size):
    BaseSignalSpec(name), m_size(size)
{

}

ArraySignalSpec::ArraySignalSpec(const ArraySignalSpec &other):
    BaseSignalSpec(other), m_size(other.m_size)
{

}

ArraySignalSpec::~ArraySignalSpec()
{

}

void ArraySignalSpec::setSize(int size)
{
    m_size = size;
}

int ArraySignalSpec::size() const
{
    return m_size;
}


AggregateSignalSpec::AggregateSignalSpec(const QString &name):
    BaseSignalSpec(name)
{

}

AggregateSignalSpec::AggregateSignalSpec(const AggregateSignalSpec &other):
    BaseSignalSpec(other), m_signals(other.m_signals), m_signalMap(other.m_signalMap)
{

}

AggregateSignalSpec::~AggregateSignalSpec()
{
    qDeleteAll(m_signals);
}

int AggregateSignalSpec::size() const
{
    return m_signals.size();
}

const BaseSignalSpec *AggregateSignalSpec::signal(int index) const
{
    return m_signals.value(index, nullptr);

}

const BaseSignalSpec *AggregateSignalSpec::signal(const QString &name) const
{
    return m_signalMap.value(name, nullptr);
}

bool AggregateSignalSpec::addSignal(BaseSignalSpec *spec)
{
    if (!spec || m_signalMap.contains(spec->name()))
        return false;
    m_signals.append(spec);
    m_signalMap.insert(spec->name(), spec);
    return true;
}

bool AggregateSignalSpec::removeSignal(int index)
{
    if (index < size()) {
        BaseSignalSpec *spec = m_signals.value(index);
        m_signalMap.remove(spec->name());
        m_signals.removeAt(index);
        delete spec;
        return true;
    }
    return false;
}

bool AggregateSignalSpec::removeSignal(const QString &name)
{
    if (!m_signalMap.contains(name))
        return false;
    BaseSignalSpec *spec = m_signalMap.value(name);
    m_signalMap.remove(name);
    m_signals.removeOne(spec);
    return true;
}
