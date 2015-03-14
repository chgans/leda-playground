#ifndef IGRIDFACTORY_H
#define IGRIDFACTORY_H

#include <QString>

class IGrid;
class IGridEditor;

class IGridFactory
{
public:
    IGridFactory();

    virtual QString id() const = 0;
    virtual QString label() const = 0;
    virtual IGrid *createGrid() const = 0; // TODO: from JSonObject
    virtual IGridEditor *createGridEditor() const = 0;
};

#endif // IGRIDFACTORY_H
