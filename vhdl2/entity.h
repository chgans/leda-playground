#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QVariant>

struct Port
{
    explicit Port(const QString &name = QString(),
                  QMetaType::Type type = QMetaType::UnknownType):
        name(name), type(type),
        m_validated(false), m_validity(false)
    {}

    QString name;
    QMetaType::Type type;

    bool isValid() const;
    QString errorMessage() const;

private:
    bool m_validated;
    bool m_validity;
    QString m_errorMessage;
};

typedef QList<Port *> PortList;

// TODO Add constants
struct Generic
{
    Generic(const QString &name = QString(),
            const QString &desc = QString(),
            const QString &unit = QString(),
            QMetaType::Type type = QMetaType::UnknownType,
            bool required = false,
            QVariant defaultValue = QVariant()):
        name(name), description(desc), unit(unit), type(type),
        required(required), defaultValue(defaultValue),
        m_validated(false), m_validity(false)
    {}
    QString name;
    QString description;
    QString unit;
    QMetaType::Type type;
    bool required;
    QVariant defaultValue;

    bool isValid() const;
    QString errorMessage() const;

private:
    bool m_validated;
    bool m_validity;
    QString m_errorMessage;
};

typedef QList<Generic *> GenericList;

struct Entity
{
    Entity(const QString &name = QString(),
           const GenericList &generics = GenericList(),
           const PortList &ports = PortList()):
        name(name), generics(generics), ports(ports),
        m_validated(false), m_validity(false)
    {}

    QString name;
    QList<Generic *> generics;
    QList<Port *> ports;

    bool isValid() const;
    QString errorMessage() const;

private:
    bool m_validated;
    bool m_validity;
    QString m_errorMessage;
};


#endif // ENTITY_H
