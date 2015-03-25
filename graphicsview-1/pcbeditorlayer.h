#ifndef PCBEDITORLAYER_H
#define PCBEDITORLAYER_H

#include <QString>
#include <QColor>

/*
 *


        Overlay, // Print
        Surface, // SolderPasteMask, SolderResistMask, GlueDotMask
        Copper, // Signal or Plane
        Dielectric, // Core, prereg,

        DrcLayerType,
        GridLayerType,
        ConnectionLayerType,
        BackgroundLayerType,
        SheetLayerType,
        BoardLayerType, // Board area
        RoomLayerType,
        StackLayerType, // Defining board region for layer stack
        HighlightLayerType, // outline all objects belonging to a net

        // TBD: logical vs physical, (+ system?)
        // TBD: Add Dielectric type (layer stack)

*/

// FIXME: rename into ViewLayer, see Altium "View Configuration"
class PcbEditorLayer
{
public:
    enum LayerType {
        NoLayerType = 0,
        MultiLayerLayerType,
        SolderPasteMaskLayerType,
        SolderResistMaskLayerType,
        GlueDotMaskLayerType,
        SilkScreenLayerType,
        SignalLayerType,
        PlaneLayerType,
        DrillGuideLayerType,
        DrillDrawingLayerType,
        KeepOutLayerType,
        MechanicalLayerType,
    };

    enum LayerGroup {
        NoLayerGroup = 0,
        SignalLayerGroup,
        PlaneLayerGroup,
        MechanicalLayerGroup,
        OtherLayerGroup,
        WorkspaceLayerGroup,
        SystemLayerGroup
    };

    enum LayerIndex { // ? + flags
        NoLayerIndex = 0,
        Layer1 = 0x00000001,
        Layer2 = 0x00000002,
        Layer3 = 0x00000004,
        Layer4 = 0x00000008,
        Layer5 = 0x00000010,
        Layer6 = 0x00000020,
        /* .. 32 */
    };

    PcbEditorLayer();
    virtual ~PcbEditorLayer();

    LayerType type() const;
    LayerGroup group() const;

    QString shortName() const;
    QString mediumName() const;
    QString longName() const;
    void setLongName(const QString &name);

    const QColor &color() const;
    void setColor(const QColor &color);
};

#endif // PCBEDITORLAYER_H
