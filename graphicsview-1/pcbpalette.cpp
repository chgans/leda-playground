#include "pcbpalette.h"

/* TODO:
 *  - Layer color vs system colors
 *  - Use layer enum from DesignLayer
 */


#include "QDebug"

PcbPalette::PcbPalette():
    m_system(false)
{
    memset(this->m_palette, qRgba(0xff, 0xff, 0xff, 0xff), sizeof(m_palette));
    //memset(this->m_palette, qintptr(-1), sizeof(m_palette));
}

PcbPalette::PcbPalette(const PcbPalette &other)
{
    memcpy(this->m_palette, other.m_palette, sizeof(m_palette));
    m_system = other.m_system;
}

QString PcbPalette::name() const
{
    return m_name;
}

void PcbPalette::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
}

QColor PcbPalette::color(PcbPalette::ColorRole role) const
{
    return QColor::fromRgba(m_palette[role]);
}

void PcbPalette::setColor(PcbPalette::ColorRole role, const QColor &color)
{
    m_palette[role] = color.rgba();
}

void PcbPalette::setSystemPalette(bool system)
{
    if (m_system == system)
        return;
    m_system = system;
}

// FIXME: return sucess/failure
void PcbPalette::loadFromSettings(QSettings &settings)
{
    settings.beginGroup("PcbPalette");
    setSystemPalette(settings.value("system", false).toBool());
    settings.beginReadArray("colors");
    for (int i=0; i<128; ++i) {
        ColorRole role = static_cast<ColorRole>(i);
        QColor color(settings.value(QString("%1").arg(i),
                                    "#ffffffff").toString());
        setColor(role, color);
    }
    settings.endArray();
    settings.endGroup();
}

void PcbPalette::saveToSettings(QSettings &settings) const
{
    settings.beginGroup("PcbPalette");
    //settings.setValue("name", m_name);
    settings.setValue("system", isSystemPalette());
    settings.beginWriteArray("colors", 128);
    for (int i=0; i<128; ++i) {
        ColorRole role = static_cast<ColorRole>(i);
        QString argb = color(role).name(QColor::HexArgb);
        settings.setValue(QString("%1").arg(i),
                          argb);
    }
    settings.endArray();
    settings.endGroup();
}

bool PcbPalette::isSystemPalette() const
{
    return m_system;
}

bool PcbPalette::operator!=(const PcbPalette &p) const
{
    return !(*this == p);
}

PcbPalette &PcbPalette::operator=(const PcbPalette &p)
{
    memcpy(this->m_palette, p.m_palette, sizeof(m_palette));
    return *this;
}

PcbPalette &PcbPalette::operator=(PcbPalette &other)
{
    memcpy(this->m_palette, other.m_palette, sizeof(m_palette));
    return *this;
}

bool PcbPalette::operator==(const PcbPalette &p) const
{
    return (memcmp(this->m_palette, p.m_palette, sizeof(m_palette)) == 0);
}

QList<PcbPalette::ColorRole> PcbPalette::allValidColorRoles() const
{
    QList<PcbPalette::ColorRole> list;
    for (int i=1; i<=124; ++i)
        list.append(static_cast<ColorRole>(i));
    return list;
}

QString PcbPalette::colorRoleLabel(PcbPalette::ColorRole role) const
{
    if (role == SignalLayer1)
        return QString("Top Layer");
    else if (SignalLayer2 <= role && role <= SignalLayer31)
        return QString("Mid-Layer %1").arg(role-1);
    else if (role == SignalLayer32)
        return QString("Bottom Layer");
    else if (PlaneLayer1 <= role && role <= PlaneLayer32)
        return QString("Internal Plane %1").arg(role - PlaneLayer1 + 1);
    else if (MechanicalLayer1 <= role && role <= MechanicalLayer32)
        return QString("Mechanical Layer %1").arg(role - MechanicalLayer1 + 1);
    switch (role) {
    case TopPasteLayer: return QStringLiteral("Top Paste");
    case BottomPasteLayer: return QStringLiteral("Bottom Paste");
    case TopSolderLayer: return QStringLiteral("Top Solder");
    case BottomSolderLayer: return QStringLiteral("Bottom Solder");
    case DrillGuideLayer: return QStringLiteral("Drill Guide");
    case KeepOutLayer: return QStringLiteral("Keep Out Layer");
    case DrillDrawingLayer: return QStringLiteral("Drill Drawing");
    case MultiLayerLayer: return QStringLiteral("Multi Layer");
    case TopOverlayLayer: return QStringLiteral("Top Overlay");
    case BottomOverlayLayer: return QStringLiteral("Bottom Overlay");
    case Selection: return QStringLiteral("Selection");
    case Connection: return QStringLiteral("Connection");
    case DrcErrorMarker: return QStringLiteral("Drc Error Marker");
    case DrcErrorDetail: return QStringLiteral("Drc Error Detail");
    case LargeGrid: return QStringLiteral("Large Grid");
    case SmallGrid: return QStringLiteral("Small Grid");
    case LineGuide: return QStringLiteral("Line Guide");
    case PadHole: return QStringLiteral("Pad Hole");
    case ViaHole: return QStringLiteral("Via Hole");
    case TopPadMaster: return QStringLiteral("Top Pad Master");
    case BottomPadMaster: return QStringLiteral("Bottom Pad Master");
    case HighLight: return QStringLiteral("Highlight");
    case BoardLine: return QStringLiteral("Board Line");
    case BoardArea: return QStringLiteral("Board Area");
    case SheetLine: return QStringLiteral("Sheet Line");
    case SheetArea: return QStringLiteral("Sheet Area");
    case WorkspaceGradientStart: return QStringLiteral("Workspace Gradient Start");
    case WorkspaceGradientEnd: return QStringLiteral("Workspace Gradient End");
    default: return QString("<!INVALID!LAYER!>");
    }

}

QString PcbPalette::colorRoleToAltiumName(PcbPalette::ColorRole role) const
{
    if (role == SignalLayer1)
        return QString("LayerColors/TopLayer");
    else if (SignalLayer2 <= role && role <= SignalLayer31)
        return QString("LayerColors/MidLayer%1").arg(role-1);
    else if (role == SignalLayer32)
        return QString("LayerColors/BottomLayer");
    else if (PlaneLayer1 <= role && role <= PlaneLayer32)
        return QString("LayerColors/InternalPlane%1").arg((role - PlaneLayer1) % 16 +1);
    else if (MechanicalLayer1 <= role && role <= MechanicalLayer32)
        return QString("LayerColors/Mechanical%1").arg((role - MechanicalLayer1) % 16 + 1);
    switch (role) {
    case TopPasteLayer: return QStringLiteral("LayerColors/TopPaste");
    case BottomPasteLayer: return QStringLiteral("LayerColors/BottomPaste");
    case TopSolderLayer: return QStringLiteral("LayerColors/TopSolder");
    case BottomSolderLayer: return QStringLiteral("LayerColors/BottomSolder");
    case TopOverlayLayer: return QStringLiteral("LayerColors/TopOverlay");
    case BottomOverlayLayer: return QStringLiteral("LayerColors/BottomOverlay");
    case DrillGuideLayer: return QStringLiteral("LayerColors/DrillGuide");
    case KeepOutLayer: return QStringLiteral("LayerColors/KeepOutLayer");
    case DrillDrawingLayer: return QStringLiteral("LayerColors/DrillDrawing");
    case MultiLayerLayer: return QStringLiteral("LayerColors/MultiLayer");
    case Connection: return QStringLiteral("LayerColors/ConnectLayer");
    // TBD: BackgroundLayer
    case DrcErrorMarker: return QStringLiteral("LayerColors/DRCErrorLayer");
    case LargeGrid: return QStringLiteral("LayerColors/GridColor1");
    case SmallGrid: return QStringLiteral("LayerColors/GridColor10");
    case PadHole: return QStringLiteral("LayerColors/PadHoleLayer");
    case ViaHole: return QStringLiteral("LayerColors/ViaHoleLayer");
    // TBD: Either LayerColors/HighlightLayer or WorkspaceColors/HighlightColor
    case HighLight: return QStringLiteral("LayerColors/HighlightLayer");
    case BoardLine: return QStringLiteral("WorkspaceColors/BoardLineColor");
    case BoardArea: return QStringLiteral("WorkspaceColors/BoardAreaColor");
    case SheetLine: return QStringLiteral("WorkspaceColors/SheetLineColor");
    case SheetArea: return QStringLiteral("WorkspaceColors/SheetAreaColor");
    case WorkspaceGradientStart: return QStringLiteral("WorkspaceColors/WorkspaceColor1");
    case WorkspaceGradientEnd: return QStringLiteral("WorkspaceColors/WorkspaceColor2");
    // TBD:
    /*
    case DrcErrorDetail: return QStringLiteral("Drc Error Detail");
    case Selection: return QStringLiteral("Selection");
    case LineGuide: return QStringLiteral("Line Guide");
    case TopPadMaster: return QStringLiteral("Top Pad Master");
    case BottomPadMaster: return QStringLiteral("Bottom Pad Master");
    */
    default: return QString("<!INVALID!LAYER!>");
    }

}

