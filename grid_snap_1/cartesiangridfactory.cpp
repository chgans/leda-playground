#include "cartesiangridfactory.h"
#include "cartesiangrid.h"
#include "cartesiangrideditor.h"

CartesianGridFactory::CartesianGridFactory()
{
}

QString CartesianGridFactory::id() const
{
    return QStringLiteral("LibreEDA.Grid.Cartesian");
}

QString CartesianGridFactory::label() const
{
    return QStringLiteral("Cartesian");
}

IGrid *CartesianGridFactory::createGrid() const
{
    return new CartesianGrid();
}

IGridEditor *CartesianGridFactory::createGridEditor() const
{
    return new CartesianGridEditor();
}
