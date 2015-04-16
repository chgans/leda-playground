#include "pcbpalettemanager.h"
#include "pcbpalette.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>
#include <QDebug>

// TODO:
//  - system vs user palettes
//  - Make the owner a friend class, the only one allow to call the constructor
//    allowing to have a parent and auto-delete
//  - Palette creation only through the manager (palettes are owned by the manager)
//
//  - DesignPalette vs Physical palette (as in DesignLayer and PhysicalLayer)
//  - Add opacity

static PcbPaletteManager *gPaletteManager = 0;

PcbPaletteManager::PcbPaletteManager(QObject *parent) :
    QObject(parent)
{
}

// TODO: destructor and cleanup

PcbPaletteManager *PcbPaletteManager::instance()
{
    if (!gPaletteManager)
        gPaletteManager = new PcbPaletteManager;
    return gPaletteManager;
}

PcbPalette *PcbPaletteManager::palette(const QString &identifier) const
{
    Q_ASSERT(m_paletteMap.contains(identifier));
    return m_paletteMap[identifier];
}

QList<PcbPalette *> PcbPaletteManager::palettes() const
{
    return m_paletteMap.values();
}

PcbPalette *PcbPaletteManager::addPalette(const QString &identifier)
{
    Q_ASSERT(!m_paletteMap.contains(identifier));
    PcbPalette *palette = new PcbPalette;
    palette->setName(identifier);
    m_paletteMap.insert(identifier, palette);
    emit paletteAdded(palette);
    return palette;
}

PcbPalette *PcbPaletteManager::addPalette(const QString &identifier, QSettings &settings)
{
    Q_ASSERT(!m_paletteMap.contains(identifier));
    PcbPalette *palette = new PcbPalette;
    palette->loadFromSettings(settings);
    palette->setName(identifier);
    m_paletteMap.insert(identifier, palette);
    emit paletteAdded(palette);
    return palette;
}

void PcbPaletteManager::removePalette(PcbPalette *palette)
{
    Q_ASSERT(m_paletteMap.contains(palette->name()));
    m_paletteMap.remove(palette->name());
    if (m_activePalette == palette) {
        setActivePalette(m_paletteMap.values().last());
    }
    emit paletteRemoved(palette);
    delete palette;
}

PcbPalette *PcbPaletteManager::activePalette() const
{
    return m_activePalette;
}

QString PcbPaletteManager::activePaletteIdentifier() const
{
    return m_activePalette->name();
}

void PcbPaletteManager::setActivePalette(PcbPalette *palette)
{
    qDebug() << "Activating palette" << palette->name();
    Q_ASSERT(m_paletteMap.contains(palette->name()));
    m_activePalette = palette;
    emit paletteActivated(palette);
}

void PcbPaletteManager::loadPalettes()
{
    qDebug() << "Loading palettes from" << m_path;
    QDir dir(m_path);
    QStringList filters;
    filters << "*.LedaPcbPalette";
    PcbPalette *palette = nullptr;
    foreach (QFileInfo fileInfo, dir.entryInfoList(filters)) {
        QString id = fileInfo.baseName();
        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;
        file.close();
        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        palette = addPalette(id, settings);
        qDebug() << "Adding" << id;
    }
    if (!m_paletteMap.isEmpty()) // Fix me: from user settings
        setActivePalette(palette);
}
