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

    foreach (const PcbPalette *palette, manager->palettes()) {
        addItem(palette);
    }
    updateCurrentIndex(manager->activePalette());
}

void ColorProfileComboBox::addItem(const PcbPalette *palette)
{
    QComboBox::addItem(palette->name(), QVariant::fromValue<const PcbPalette *>(palette));
}

void ColorProfileComboBox::removeItem(const PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (itemData(i).value<const PcbPalette *>() == palette) {
            QComboBox::removeItem(i);
            return;
        }
    }
}

void ColorProfileComboBox::updateItem(const PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (itemData(i).value<const PcbPalette *>() == palette) {
            setItemText(i, palette->name());
            return;
        }
    }
}

void ColorProfileComboBox::activatePalette(int index)
{
    PcbPaletteManager::instance()->setActivePalette(paletteAt(index));
}

void ColorProfileComboBox::updateCurrentIndex(const PcbPalette *palette)
{
    setCurrentIndex(index(palette));
}

int ColorProfileComboBox::index(const PcbPalette *palette)
{
    for (int i = 0; i < count(); i++) {
        if (paletteAt(i) == palette) {
            return i;
        }
    }
    Q_ASSERT(false);
    return 0;
}

const PcbPalette *ColorProfileComboBox::paletteAt(int index)
{
    return itemData(index).value<const PcbPalette *>();
}
