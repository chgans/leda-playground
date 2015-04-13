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
            this, [this](PcbPalette *palette) {
        qDebug() << "Updating index";
        action(palette)->setChecked(true);
    });

    foreach (PcbPalette *palette, manager->palettes()) {
        addPalette(palette);
    }
}

void ColorProfileToolButton::addPalette(PcbPalette *palette)
{
    QAction *action = m_actionGroup->addAction(palette->name());
    action->setData(QVariant::fromValue<PcbPalette *>(palette));
    action->setCheckable(true);
    action->setChecked(true);
    m_menu->addAction(action);
}

void ColorProfileToolButton::removePalette(PcbPalette *palette)
{
    m_actionGroup->removeAction(action(palette));
}

void ColorProfileToolButton::updatePalette(PcbPalette *palette)
{
    action(palette)->setText(palette->name());
}

QAction *ColorProfileToolButton::action(PcbPalette *palette)
{
    foreach (QAction *action, m_actionGroup->actions()) {
        if (action->data().value<PcbPalette *>() == palette) {
            return action;
        }
    }
    Q_ASSERT(false);
    return nullptr;
}

PcbPalette *ColorProfileToolButton::palette(QAction *action)
{
    return action->data().value<PcbPalette *>();
    Q_ASSERT(false);
    return nullptr;
}
