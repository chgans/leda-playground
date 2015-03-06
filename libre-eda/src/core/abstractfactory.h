#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "singleton.h"
#include <QtAlgorithms>
#include <QString>
#include <QMap>

template <class T>
class AbstractFactorySingleton: public Singleton <T>
{

};

template<class Object, class Creator>
class AbstractFactory: public Singleton<AbstractFactory<Object, Creator>>
{
public:
    ~AbstractFactory()
    {
        qDeleteAll(m_idToCreator.values());
    }

    void registerCreator(Creator *creator)
    {
        foreach(const QString &id, creator->typeIds()) {
            m_idToCreator[id] = creator;
        }
    }

    Object *create(const QString &id)
    {
        if (!m_idToCreator.contains(id))
            return nullptr;
        else
            return m_idToCreator->create(id);
    }

private:
    AbstractFactory();
    QMap<QString, Creator *> m_idToCreator;
};

#endif // ABSTRACTFACTORY_H
