#ifndef DESIGNUNIT_H
#define DESIGNUNIT_H

#include "entity.h"
#include "architecture.h"
#include "configuration.h"

class DesignUnit
{
public:
    DesignUnit()
    {}

    QString name;
    QList<Entity *> entities;
    QList<Architecture *> architectures;
    QList<Configuration *> configurations;
};

#endif // DESIGNUNIT_H
