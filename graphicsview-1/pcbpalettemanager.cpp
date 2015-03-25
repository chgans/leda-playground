#include "pcbpalettemanager.h"
#include "pcbpalette.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>

// TODO:
//  - system vs user palettes
//  - Default palette: in a file or hard coded?
//  - Make the owner a friend class, the only one allow to call the constructor
//    allowing to have a parent and auto-delete
//  - Palette creation only through the manager (palettes are owned by the manager)
//
//  - DesignPalette vs Physical palette (as in DesignLayer and PhysicalLayer)
//  - Add opacity

static PcbPalette *gDefaultPalette = 0;
static PcbPaletteManager *gPaletteManager = 0;
static const char DEFAULT[] = "Default";

PcbPaletteManager::PcbPaletteManager(QObject *parent) :
    QObject(parent)
{
}

// TODO: destructor and cleanup

PcbPaletteManager *PcbPaletteManager::instance()
{
    if (!gPaletteManager)
        gPaletteManager = new PcbPaletteManager;
    if (!gDefaultPalette) {
        gDefaultPalette = new PcbPalette(); // TODO: new PcbPalette(gPcbPaletteManager);
        gPaletteManager->mPaletteMap.insert(QString(DEFAULT), *gDefaultPalette);
        gPaletteManager->mActivePaletteId = DEFAULT;
    }
    return gPaletteManager;
}

const PcbPalette &PcbPaletteManager::palette(const QString &identifier) const
{
    QString id(identifier);
    if (!mPaletteMap.keys().contains(identifier))
        id = QString(DEFAULT);
    QMap<QString, PcbPalette>::const_iterator iter;
    iter = mPaletteMap.find(id);
    return iter.value();
}

QStringList PcbPaletteManager::paletteIdentifiers() const
{
    return mPaletteMap.keys();
}

void PcbPaletteManager::setPalette(const QString &identifier, PcbPalette &palette)
{
    if (!mPaletteMap.keys().contains(identifier) ||
            identifier == QString(DEFAULT) ||
            mPaletteMap[identifier] == palette)
        return;
    mPaletteMap[identifier] = palette;
    emit paletteChanged(identifier);
}

void PcbPaletteManager::addPalette(const QString &identifier, PcbPalette &palette)
{
    if (mPaletteMap.keys().contains(identifier) ||
            identifier.isEmpty() ||
            identifier == QString(DEFAULT))
        return;
    mPaletteMap.insert(identifier, palette);
    emit paletteAdded(identifier);
}

void PcbPaletteManager::removePalette(const QString &identifier)
{
    if (!mPaletteMap.keys().contains(identifier) ||
            identifier == QString(DEFAULT))
        return;
    mPaletteMap.remove(identifier);
    if (mActivePaletteId == identifier) {
        setActivePalette(mPaletteMap.keys().last());
    }
    emit paletteRemoved(identifier);
}

const PcbPalette &PcbPaletteManager::activePalette() const
{
    QMap<QString, PcbPalette>::const_iterator iter;
    iter = mPaletteMap.find(mActivePaletteId);
    return iter.value();
}

const QString &PcbPaletteManager::activePaletteIdentifier() const
{
    return mActivePaletteId;
}

void PcbPaletteManager::setActivePalette(const QString &identifier)
{
    if (identifier.isEmpty() ||
            mActivePaletteId == identifier ||
            !mPaletteMap.keys().contains(identifier))
        return;
    mActivePaletteId = identifier;
    emit paletteActivated(identifier);
}

void PcbPaletteManager::loadPalettes()
{
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
        PcbPalette palette;
        palette.loadFromSettings(settings);
        addPalette(id, palette);
    }
    if (!mPaletteMap.isEmpty()) // Fix me: from user settings
        setActivePalette(mPaletteMap.keys().first());
}
