#ifndef PCBPALETTE_H
#define PCBPALETTE_H

#include <QColor>
#include <QVariant>
#include <QSettings>

class PcbPaletteManager;

// 2D color profile

class PcbPalette
{
public:

    // TBD: use role + index
    enum ColorRole {
        NoRole = 0,
        SignalLayer1 = 1,
        SignalLayer2 = 2,
        SignalLayer3 = 3,
        SignalLayer4 = 4,
        SignalLayer5 = 5,
        SignalLayer6 = 6,
        SignalLayer7 = 7,
        SignalLayer8 = 8,
        SignalLayer9 = 9,
        SignalLayer10 = 10,
        SignalLayer11 = 11,
        SignalLayer12 = 12,
        SignalLayer13 = 13,
        SignalLayer14 = 14,
        SignalLayer15 = 15,
        SignalLayer16 = 16,
        SignalLayer17 = 17,
        SignalLayer18 = 18,
        SignalLayer19 = 19,
        SignalLayer20 = 20,
        SignalLayer21 = 21,
        SignalLayer22 = 22,
        SignalLayer23 = 23,
        SignalLayer24 = 24,
        SignalLayer25 = 25,
        SignalLayer26 = 26,
        SignalLayer27 = 27,
        SignalLayer28 = 28,
        SignalLayer29 = 29,
        SignalLayer30 = 30,
        SignalLayer31 = 31,
        SignalLayer32 = 32,
        TopLayer = SignalLayer1,
        MidLayer1 = SignalLayer2,
        MidLayer2 = SignalLayer3,
        MidLayer3 = SignalLayer4,
        MidLayer4 = SignalLayer5,
        MidLayer5 = SignalLayer6,
        MidLayer6 = SignalLayer7,
        MidLayer7 = SignalLayer8,
        MidLayer8 = SignalLayer9,
        MidLayer9 = SignalLayer10,
        MidLayer10 = SignalLayer11,
        MidLayer11 = SignalLayer12,
        MidLayer12 = SignalLayer13,
        MidLayer13 = SignalLayer14,
        MidLayer14 = SignalLayer15,
        MidLayer15 = SignalLayer16,
        MidLayer16 = SignalLayer17,
        MidLayer17 = SignalLayer18,
        MidLayer18 = SignalLayer19,
        MidLayer19 = SignalLayer20,
        MidLayer20 = SignalLayer21,
        MidLayer21 = SignalLayer22,
        MidLayer22 = SignalLayer23,
        MidLayer23 = SignalLayer24,
        MidLayer24 = SignalLayer25,
        MidLayer25 = SignalLayer26,
        MidLayer26 = SignalLayer27,
        MidLayer27 = SignalLayer28,
        MidLayer28 = SignalLayer29,
        MidLayer29 = SignalLayer30,
        MidLayer30 = SignalLayer31,
        BottomLayer = SignalLayer32,
        PlaneLayer1 = 33,
        PlaneLayer2 = 34,
        PlaneLayer3 = 35,
        PlaneLayer4 = 36,
        PlaneLayer5 = 37,
        PlaneLayer6 = 38,
        PlaneLayer7 = 39,
        PlaneLayer8 = 40,
        PlaneLayer9 = 41,
        PlaneLayer10 = 42,
        PlaneLayer11 = 43,
        PlaneLayer12 = 44,
        PlaneLayer13 = 45,
        PlaneLayer14 = 46,
        PlaneLayer15 = 47,
        PlaneLayer16 = 48,
        PlaneLayer17 = 49,
        PlaneLayer18 = 50,
        PlaneLayer19 = 51,
        PlaneLayer20 = 52,
        PlaneLayer21 = 53,
        PlaneLayer22 = 54,
        PlaneLayer23 = 55,
        PlaneLayer24 = 56,
        PlaneLayer25 = 57,
        PlaneLayer26 = 58,
        PlaneLayer27 = 59,
        PlaneLayer28 = 60,
        PlaneLayer29 = 61,
        PlaneLayer30 = 62,
        PlaneLayer31 = 63,
        PlaneLayer32 = 64,
        MechanicalLayer1 = 65,
        MechanicalLayer2 = 66,
        MechanicalLayer3 = 67,
        MechanicalLayer4 = 68,
        MechanicalLayer5 = 69,
        MechanicalLayer6 = 70,
        MechanicalLayer7 = 71,
        MechanicalLayer8 = 72,
        MechanicalLayer9 = 73,
        MechanicalLayer10 = 74,
        MechanicalLayer11 = 75,
        MechanicalLayer12 = 76,
        MechanicalLayer13 = 77,
        MechanicalLayer14 = 78,
        MechanicalLayer15 = 79,
        MechanicalLayer16 = 80,
        MechanicalLayer17 = 81,
        MechanicalLayer18 = 82,
        MechanicalLayer19 = 83,
        MechanicalLayer20 = 84,
        MechanicalLayer21 = 85,
        MechanicalLayer22 = 86,
        MechanicalLayer23 = 87,
        MechanicalLayer24 = 88,
        MechanicalLayer25 = 89,
        MechanicalLayer26 = 90,
        MechanicalLayer27 = 91,
        MechanicalLayer28 = 92,
        MechanicalLayer29 = 93,
        MechanicalLayer30 = 94,
        MechanicalLayer31 = 95,
        MechanicalLayer32 = 96,
        TopPasteLayer = 97,
        BottomPasteLayer = 98,
        TopSolderLayer = 99,
        BottomSolderLayer = 100,
        TopCoverLayLayer = TopSolderLayer,
        BottomCoverlayLayer = BottomSolderLayer,
        DrillGuideLayer = 101,
        KeepOutLayer = 102,
        DrillDrawingLayer = 103,
        MultiLayerLayer = 104,
        TopOverlayLayer = 105,
        BottomOverlayLayer = 106,

        Selection = 107,
        Connection = 108,
        DrcErrorMarker = 109,
        DrcErrorDetail = 110,
        LargeGrid = 111,
        SmallGrid = 112,
        LineGuide = 113,
        PadHole = 114,
        ViaHole = 115,
        TopPadMaster = 116, // altium legacy?
        BottomPadMaster = 117,  // altium legacy?
        HighLight = 118,
        BoardLine = 119,
        BoardArea = 120, // different from board area in board def mode
        SheetLine = 121,
        SheetArea = 122,
        WorkspaceGradientStart = 123,
        WorkspaceGradientEnd = 124,

        FirstRole = SignalLayer1,
        LastRole = WorkspaceGradientEnd
    };

    PcbPalette();
    PcbPalette(const PcbPalette &other);

    QString name() const;

    QColor color(ColorRole role) const;

    void loadFromSettings(QSettings &settings);
    void saveToSettings(QSettings &settings) const;

    bool isSystemPalette() const;

    //TBD: operator QVariant() const;
    bool operator!=(const PcbPalette & p) const;
    PcbPalette &operator=(const PcbPalette &p);
    PcbPalette &operator=(PcbPalette &other);
    bool operator==(const PcbPalette &p) const;

    QList<ColorRole> allValidColorRoles() const;
    QString colorRoleLabel(ColorRole role) const;
    QString colorRoleToAltiumName(ColorRole role) const;

private:
    friend class PcbPaletteManager;
    QString m_name;
    bool m_system;
    QRgb m_palette[128];
    void setName(const QString &name);
    void setColor(ColorRole role, const QColor &color);
    void setSystemPalette(bool system);
};

Q_DECLARE_METATYPE(PcbPalette*)
Q_DECLARE_METATYPE(const PcbPalette*)

// TBD:
// QDataStream &operator<<(QDataStream &s, const PcbPalette &p);
// QDataStream &operator>>(QDataStream &s, PcbPalette &p);

#endif // PCBPALETTE_H
