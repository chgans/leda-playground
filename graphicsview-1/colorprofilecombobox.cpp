#include "colorprofilecombobox.h"

#include "pcbpalette.h"
#include "pcbpalettemanager.h"

#include <QDebug>

ColorProfileComboBox::ColorProfileComboBox(QWidget *parent) :
    QComboBox(parent)
{
    PcbPaletteManager *manager = PcbPaletteManager::instance();
    connect(manager, &PcbPaletteManager::paletteAdded,
            this, &ColorProfileComboBox::addItem);
    connect(manager, &PcbPaletteManager::paletteRemoved,
            this, &ColorProfileComboBox::removeItem);
    connect(manager, &PcbPaletteManager::paletteChanged,
            this, &ColorProfileComboBox::updateItem);
    connect(manager, &PcbPaletteManager::paletteActivated,
            this, &ColorProfileComboBox::updateCurrentIndex);
    void (QComboBox::*signal)(int) = &QComboBox::currentIndexChanged;
    connect(this, signal,
            this, &ColorProfileComboBox::activatePalette);

    foreach (PcbPalette *palette, manager->palettes()) {
        addItem(palette);
    }
    updateCurrentIndex(manager->activePalette());
}

void ColorProfileComboBox::addItem(PcbPalette *palette)
{
    QComboBox::addItem(palette->name(), QVariant::fromValue<PcbPalette *>(palette));
}

void ColorProfileComboBox::removeItem(PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (itemData(i).value<PcbPalette *>() == palette) {
            QComboBox::removeItem(i);
            return;
        }
    }
}

void ColorProfileComboBox::updateItem(PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (itemData(i).value<PcbPalette *>() == palette) {
            setItemText(i, palette->name());
            return;
        }
    }
}

void ColorProfileComboBox::activatePalette(int index)
{
    PcbPaletteManager::instance()->setActivePalette(paletteAt(index));
}

void ColorProfileComboBox::updateCurrentIndex(PcbPalette *palette)
{
    setCurrentIndex(index(palette));
}

int ColorProfileComboBox::index(PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (paletteAt(i) == palette) {
            return i;
        }
    }
    Q_ASSERT(false);
    return 0;
}

PcbPalette *ColorProfileComboBox::paletteAt(int index)
{
    return itemData(index).value<PcbPalette *>();
}
