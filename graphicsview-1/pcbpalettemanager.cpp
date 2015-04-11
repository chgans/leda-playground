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

QStringList PcbPaletteManager::paletteIdentifiers() const
{
    return mPaletteMap.keys();
}

void PcbPaletteManager::setPalette(const QString &identifier, PcbPalette *palette)
{
    Q_ASSERT(mPaletteMap.contains(identifier));
    mPaletteMap[identifier] = palette;
    emit paletteChanged(identifier);
}

void PcbPaletteManager::addPalette(const QString &identifier, PcbPalette *palette)
{
    Q_ASSERT(!mPaletteMap.contains(identifier));
    mPaletteMap.insert(identifier, palette);
    emit paletteAdded(identifier);
}

void PcbPaletteManager::removePalette(const QString &identifier)
{
    Q_ASSERT(mPaletteMap.contains(identifier));
    mPaletteMap.remove(identifier);
    if (mActivePaletteId == identifier) {
        setActivePalette(mPaletteMap.keys().last());
    }
    emit paletteRemoved(identifier);
}

PcbPalette *PcbPaletteManager::activePalette() const
{
    return mPaletteMap[mActivePaletteId];
}

QString PcbPaletteManager::activePaletteIdentifier() const
{
    return mActivePaletteId;
}

void PcbPaletteManager::setActivePalette(const QString &identifier)
{
    Q_ASSERT(mPaletteMap.contains(identifier));
    mActivePaletteId = identifier;
    emit paletteActivated(identifier);
}

void PcbPaletteManager::loadPalettes()
{
    qDebug() << "Loading palettes from" << mPath;
    QDir dir(mPath);
    QStringList filters;
    filters << "*.LedaPcbPalette";
    foreach (QFileInfo fileInfo, dir.entryInfoList(filters)) {
        QString id = fileInfo.baseName();
        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;
        file.close();
        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        PcbPalette *palette = new PcbPalette;
        palette->loadFromSettings(settings);
        qDebug() << "Adding" << id;
        addPalette(id, palette);
    }
    if (!mPaletteMap.isEmpty()) // Fix me: from user settings
        setActivePalette(mPaletteMap.keys().first());
}
