#include "polargridfactory.h"
#include "polargrid.h"
#include "polargrideditor.h"

PolarGridFactory::PolarGridFactory()
{
}

QString PolarGridFactory::id() const
{
    return QStringLiteral("LibreEDA.Grid.Polar");
}

QString PolarGridFactory::label() const
{
    return QStringLiteral("Polar");
}

IGrid *PolarGridFactory::createGrid() const
{
    return new PolarGrid();
}

IGridEditor *PolarGridFactory::createGridEditor() const
{
    return new PolarGridEditor();
}
