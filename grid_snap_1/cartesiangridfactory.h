#ifndef CARTESIANGRIDFACTORY_H
#define CARTESIANGRIDFACTORY_H

#include "igridfactory.h"

class CartesianGridFactory : public IGridFactory
{
public:
    CartesianGridFactory();

    QString id() const;
    QString label() const;
    IGrid *createGrid() const;
    IGridEditor *createGridEditor() const;
};

#endif // CARTESIANGRIDFACTORY_H
