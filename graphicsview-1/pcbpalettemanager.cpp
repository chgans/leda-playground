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
    Q_ASSERT(mPaletteMap.contains(identifier));
    return mPaletteMap[identifier];
}

QList<PcbPalette *> PcbPaletteManager::palettes() const
{
    return mPaletteMap.values();
}

PcbPalette *PcbPaletteManager::addPalette(const QString &identifier)
{
    Q_ASSERT(!mPaletteMap.contains(identifier));
    PcbPalette *palette = new PcbPalette;
    palette->setName(identifier);
    mPaletteMap.insert(identifier, palette);
    emit paletteAdded(palette);
    return palette;
}

PcbPalette *PcbPaletteManager::addPalette(const QString &identifier, QSettings &settings)
{
    Q_ASSERT(!mPaletteMap.contains(identifier));
    PcbPalette *palette = new PcbPalette;
    palette->loadFromSettings(settings);
    palette->setName(identifier);
    mPaletteMap.insert(identifier, palette);
    emit paletteAdded(palette);
    return palette;
}

void PcbPaletteManager::removePalette(PcbPalette *palette)
{
    Q_ASSERT(mPaletteMap.contains(palette->name()));
    mPaletteMap.remove(palette->name());
    if (mActivePalette == palette) {
        setActivePalette(mPaletteMap.values().last());
    }
    emit paletteRemoved(palette);
    delete palette;
}

PcbPalette *PcbPaletteManager::activePalette() const
{
    return mActivePalette;
}

QString PcbPaletteManager::activePaletteIdentifier() const
{
    return mActivePalette->name();
}

void PcbPaletteManager::setActivePalette(PcbPalette *palette)
{
    Q_ASSERT(mPaletteMap.contains(palette->name()));
    mActivePalette = palette;
    emit paletteActivated(palette);
}

void PcbPaletteManager::loadPalettes()
{
    qDebug() << "Loading palettes from" << mPath;
    QDir dir(mPath);
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
    if (!mPaletteMap.isEmpty()) // Fix me: from user settings
        setActivePalette(palette);
}
