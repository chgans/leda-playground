#ifndef POLARGRIDFACTORY_H
#define POLARGRIDFACTORY_H

#include "igridfactory.h"

class PolarGridFactory : public IGridFactory
{
public:
    PolarGridFactory();

    QString id() const;
    QString label() const;
    IGrid *createGrid() const;
    IGridEditor *createGridEditor() const;
};

#endif // POLARGRIDFACTORY_H
