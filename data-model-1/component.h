#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

class Component : public QObject
{
    Q_OBJECT
public:
    explicit Component(QObject *parent = 0);

signals:

public slots:

};

class Port {
    Id id;
    Direction dir;
    SignalType type;
};

class Attribute {
    Id id;
    DataType type;
};

class Signal {
    Id id;
    SignalType type;
};

#endif // COMPONENT_H
