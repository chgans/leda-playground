#ifndef PCBPHYSICALBOARD_H
#define PCBPHYSICALBOARD_H

#include <QList>
#include "pcbphysicallayer.h"

class PcbPhysicalBoard
{
public:
    PcbPhysicalBoard();

    // shape
    // cutoutShapes
    // layer stack

    void setLayerStack(const QList<PcbPhysicalLayer> &stack);
    QList<PcbPhysicalLayer> layerStack() const;

protected:
    QList<PcbPhysicalLayer> mLayerStack;
};

#endif // PCBPHYSICALBOARD_H
