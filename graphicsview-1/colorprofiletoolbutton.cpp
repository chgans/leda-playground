#include "colorprofiletoolbutton.h"

#include "pcbpalette.h"
#include "pcbpalettemanager.h"

#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include <QDebug>

ColorProfileToolButton::ColorProfileToolButton(QWidget *parent) :
    QToolButton(parent)
{

    setText("CP");
    setAutoRaise(true);
    setToolButtonStyle(Qt::ToolButtonTextOnly);

    m_menu = new QMenu(this);
    m_menu->addAction("Color profiles...");
    m_menu->addSeparator();

    m_actionGroup = new QActionGroup(this);
    m_actionGroup->setExclusive(true);
    connect(m_actionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        qDebug() << "activating";
        PcbPaletteManager *manager = PcbPaletteManager::instance();
        manager->setActivePalette(palette(action));
    });

    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);

    PcbPaletteManager *manager = PcbPaletteManager::instance();
    connect(manager, &PcbPaletteManager::paletteAdded,
            this, &ColorProfileToolButton::addPalette);
    connect(manager, &PcbPaletteManager::paletteRemoved,
            this, &ColorProfileToolButton::removePalette);
    connect(manager, &PcbPaletteManager::paletteChanged,
            this, &ColorProfileToolButton::updatePalette);
    connect(manager, &PcbPaletteManager::paletteActivated,
            this, [this](const PcbPalette *palette) {
        qDebug() << "Updating index";
        action(palette)->setChecked(true);
    });

    foreach (const PcbPalette *palette, manager->palettes()) {
        addPalette(palette);
    }


}

void ColorProfileToolButton::addPalette(const PcbPalette *palette)
{
    QAction *action = m_actionGroup->addAction(palette->name());
    action->setData(QVariant::fromValue<const PcbPalette *>(palette));
    action->setCheckable(true);
    action->setChecked(true);
    m_menu->addAction(action);
}

void ColorProfileToolButton::removePalette(const PcbPalette *palette)
{
    m_actionGroup->removeAction(action(palette));
}

void ColorProfileToolButton::updatePalette(const PcbPalette *palette)
{
    action(palette)->setText(palette->name());
}

QAction *ColorProfileToolButton::action(const PcbPalette *palette)
{
    foreach (QAction *action, m_actionGroup->actions()) {
        if (action->data().value<const PcbPalette *>() == palette) {
            return action;
        }
    }
    Q_ASSERT(false);
    return nullptr;
}

const PcbPalette *ColorProfileToolButton::palette(QAction *action)
{
    return action->data().value<const PcbPalette *>();
    Q_ASSERT(false);
    return nullptr;
}
