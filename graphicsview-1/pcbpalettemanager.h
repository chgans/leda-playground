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

    QStringList paletteIdentifiers() const;

    PcbPalette *palette(const QString &identifier) const;
    void setPalette(const QString &identifier,
                    PcbPalette *palette);

    void addPalette(const QString &identifier,
                    PcbPalette *palette);
    void removePalette(const QString &identifier);

    PcbPalette *activePalette() const;
    QString activePaletteIdentifier() const;

    void setActivePalette(const QString &identifier);

    // TODO: system, user and project path?
    QString palettesPath() const { return mPath; }
    void setPalettesPath(const QString &path) { mPath = path; }

    void loadPalettes();

signals:
    void paletteChanged(const QString &identifier);
    void paletteAdded(const QString &identifier);
    void paletteRemoved(const QString &identifier);
    void paletteActivated(const QString &identifier);

public slots:

private:
    QString mPath;
    QMap<QString, PcbPalette*> mPaletteMap;
    QString mActivePaletteId;
};

#endif // PCBPALETTEMANAGER_H
