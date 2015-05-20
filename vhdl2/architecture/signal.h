#ifndef SIGNAL_H
#define SIGNAL_H

#include <QString>
#include <QMetaType>

namespace architecture
{

struct Signal
{
    Signal(const QString &name = QString(),
           QMetaType::Type type = QMetaType::UnknownType):
        name(name),
        type(type)
    {}

    QString name;
    QMetaType::Type type;
};

}
#endif // SIGNAL_H
