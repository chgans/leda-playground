#ifndef LEDA_H
#define LEDA_H

namespace Le {
    enum ElectricalPortType {
        UnspecifiedPortType = 0,
        InPortType,
        OutPortType,
        InOutPortType,
        TriStatePortType,
        PassivePortType,
        PowerInPortType,
        PowerOutPortType,
        OpenCollectorType,
        OpenEmitterType
    };

    enum GraphicalPortDecoration {
        NoPortDecoration = 0,
        InvertedPortDecoration,
        ClockPortDecoration,
        InvertedClockPortDecoration,
        InputLowPortDecoration,
        OutputLowPortDecoration,
        FallingEdgeClockPortDecoration,
        NonLogicPortDecoration
    };
}
#endif // LEDA_H

