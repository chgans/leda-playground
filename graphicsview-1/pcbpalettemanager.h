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

    QList<PcbPalette *> palettes() const;

    PcbPalette *palette(const QString &identifier) const;

    PcbPalette *addPalette(const QString &identifier);
    PcbPalette *addPalette(const QString &identifier, QSettings &settings);
    void removePalette(PcbPalette *palette);

     PcbPalette *activePalette() const;
    QString activePaletteIdentifier() const;

    void setActivePalette(PcbPalette *palette);

    // TODO: system, user and project path?
    QString palettesPath() const { return m_path; }
    void setPalettesPath(const QString &path) { m_path = path; }

    void loadPalettes();

signals:
    void paletteChanged(PcbPalette *palette);
    void paletteAdded(PcbPalette *palette);
    void paletteRemoved(PcbPalette *palette);
    void paletteActivated(PcbPalette *palette);

public slots:

private:
    QString m_path;
    QMap<QString, PcbPalette*> m_paletteMap;
    PcbPalette *m_activePalette;
};

#endif // PCBPALETTEMANAGER_H
