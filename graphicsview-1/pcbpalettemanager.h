#ifndef PCBPALETTEMANAGER_H
#define PCBPALETTEMANAGER_H

#include "pcbpalette.h"

#include <QObject>
#include <QMap>
#include <QString>

class PcbPaletteManager : public QObject
{
    Q_OBJECT

public:
    explicit PcbPaletteManager(QObject *parent = 0);

    static PcbPaletteManager *instance();

    QList<const PcbPalette *> palettes() const;

    const PcbPalette *palette(const QString &identifier) const;

    const PcbPalette *addPalette(const QString &identifier);
    const PcbPalette *addPalette(const QString &identifier, QSettings &settings);
    void removePalette(const PcbPalette *palette);

    const PcbPalette *activePalette() const;
    QString activePaletteIdentifier() const;

    void setActivePalette(const PcbPalette *palette);

    // TODO: system, user and project path?
    QString palettesPath() const { return mPath; }
    void setPalettesPath(const QString &path) { mPath = path; }

    void loadPalettes();

signals:
    void paletteChanged(const PcbPalette *palette);
    void paletteAdded(const PcbPalette *palette);
    void paletteRemoved(const PcbPalette *palette);
    void paletteActivated(const PcbPalette *palette);

public slots:

private:
    QString mPath;
    QMap<QString, PcbPalette*> mPaletteMap;
    const PcbPalette *mActivePalette;
};

#endif // PCBPALETTEMANAGER_H
