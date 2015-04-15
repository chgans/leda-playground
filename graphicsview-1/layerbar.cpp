#include "layerbar.h"

#include "designlayer.h"
#include "designlayermanager.h"
#include "pcbpalette.h"
#include "pcbpalettemanager.h"

#include <QAction>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QTabBar>
#include <QToolButton>
#include <QMenu>
#include <QKeySequence>

#include <QDebug>

static QIcon createColorIcon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

LayerBar::LayerBar(QWidget *parent) : QWidget(parent)
{
    m_paletteManager = PcbPaletteManager::instance();
    m_layerManager = DesignLayerManager::instance();

    createConfigToolButton();
    createTabBar();
    createActions();
    createMenus();

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_configToolButton);
    mainLayout->addWidget(m_tabBar);
    setLayout(mainLayout);

    repopulateLayerTabs(m_layerManager->layerSet(DesignLayerSet::All));
    updateTabIcons();
    updateLayerIcon();
    populateConfigMenu();

    connectActions();
    connectTabBar();
    foreach (PcbPalette *palette, m_paletteManager->palettes())
        addPalette(palette);
    setActivePalette(m_paletteManager->activePalette());
    connectPaletteManager();
    foreach (DesignLayerSet *set, m_layerManager->allLayerSets())
        addLaterSet(set);
    setActiveLayerSet(m_layerManager->allLayerSets().first()); // FIXME
    connectLayerManager();
}

LayerBar::~LayerBar()
{
}

void LayerBar::activateLayer(int tabIndex)
{
    // TODO: emit activateLayerRequested (We don't want to touch the view here)
    DesignLayer *layer = m_tabBar->tabData(tabIndex).value<DesignLayer *>();
    updateLayerIcon();
    qDebug() << "Activating layer" << layer->stackPosition() << layer->name();
    //m_view->activateLayer(layer->stackPosition());
}

void LayerBar::activateNextLayer()
{
    activateLayer((m_tabBar->currentIndex() + 1 ) % m_tabBar->count());
}

void LayerBar::activatePreviousLayer()
{
    activateLayer((m_tabBar->currentIndex() - 1 ) % m_tabBar->count());
}

void LayerBar::activateNextSignalLayer()
{
    activateNextLayer();
}

void LayerBar::activatePreviousSignalLayer()
{
    activatePreviousLayer();
}

void LayerBar::setActivePalette(PcbPalette *palette)
{
    updateTabIcons();
    updateLayerIcon();

    foreach (QAction *action, m_colorActionGroup->actions()) {
        if (action->data().value<PcbPalette *>() == palette) {
            action ->setChecked(true);
            return;
        }
    }
    Q_ASSERT(false);
}

void LayerBar::onPaletteChanged(PcbPalette *palette)
{
    Q_UNUSED(palette);
}

void LayerBar::addPalette(PcbPalette *palette)
{
    QAction *action = new QAction(palette->name(), this);
    action->setData(QVariant::fromValue<PcbPalette *>(palette));
    action->setCheckable(true);
    m_colorActionGroup->addAction(action);
    m_colorMenu->addAction(action);
}

void LayerBar::removePalette(PcbPalette *palette)
{
    foreach (QAction *action, m_colorActionGroup->actions()) {
        if (action->data().value<PcbPalette *>() == palette) {
            m_colorActionGroup->removeAction(action);
            return;
        }
    }
    // Should not happen
    Q_ASSERT(false);
}

void LayerBar::setActiveLayerSet(DesignLayerSet *set)
{
    disconnectTabBar();
    repopulateLayerTabs(set);
    updateTabIcons();
    updateLayerIcon();
    connectTabBar();

    foreach (QAction *action, m_setActionGroup->actions()) {
        if (action->data().value<DesignLayerSet *>() == set) {
            action->setChecked(true);
            return;
        }
    }
    // Should not happen
    Q_ASSERT(false);
}

void LayerBar::onLayerSetChanged(DesignLayerSet *set)
{
    Q_UNUSED(set);
    disconnectTabBar();
    repopulateLayerTabs(set);
    updateTabIcons();
    updateLayerIcon();
    connectTabBar();
}

void LayerBar::addLaterSet(DesignLayerSet *set)
{
    QAction *action = new QAction(set->effectiveName(), this);
    action->setData(QVariant::fromValue<DesignLayerSet *>(set));
    action->setCheckable(true);
    m_setActionGroup->addAction(action);
    m_setMenu->addAction(action);
}

void LayerBar::removeLayerSet(DesignLayerSet *set)
{
    foreach (QAction *action, m_setActionGroup->actions()) {
        if (action->data().value<DesignLayerSet *>() == set) {
            m_setActionGroup->removeAction(action);
            return;
        }
    }
    // Should not happen
    Q_ASSERT(false);
}

void LayerBar::connectPaletteManager()
{
    connect(m_paletteManager, &PcbPaletteManager::paletteActivated,
            this, &LayerBar::setActivePalette);
    connect(m_paletteManager, &PcbPaletteManager::paletteChanged,
            this, &LayerBar::onPaletteChanged);
    connect(m_paletteManager, &PcbPaletteManager::paletteAdded,
            this, &LayerBar::addPalette);
    connect(m_paletteManager, &PcbPaletteManager::paletteRemoved,
            this, &LayerBar::removePalette);
}

void LayerBar::disconnectPaletteManager()
{
    m_paletteManager->disconnect(this);
}

void LayerBar::connectLayerManager()
{
    connect(m_layerManager, &DesignLayerManager::layerSetChanged,
            this, &LayerBar::onLayerSetChanged);
    connect(m_layerManager, &DesignLayerManager::layerSetAdded,
            this, &LayerBar::addLaterSet);
    connect(m_layerManager, &DesignLayerManager::layerSetRemoved,
            this, &LayerBar::removeLayerSet);
}

void LayerBar::disconnectLayerManager()
{
    m_layerManager->disconnect(this);
}

void LayerBar::createTabBar()
{
    m_tabBar = new QTabBar();
    m_tabBar->setShape(QTabBar::RoundedSouth);
    m_tabBar->setDrawBase(false);
}

void LayerBar::createConfigToolButton()
{
    m_configToolButton = new QToolButton;
    m_configToolButton->setAutoRaise(true);
    m_configToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_configToolButton->setPopupMode(QToolButton::InstantPopup);
}

void LayerBar::updateTabIcons()
{
    PcbPalette *palette = m_paletteManager->activePalette();
    for (int tabIndex = 0; tabIndex < m_tabBar->count(); tabIndex++) {
        DesignLayer *layer = m_tabBar->tabData(tabIndex).value<DesignLayer *>();
        QColor color = palette->color(PcbPalette::ColorRole(layer->stackPosition() + 1));
        layer->setColor(color);
        m_tabBar->setTabIcon(tabIndex, createColorIcon(layer->color()));
    }
}

void LayerBar::updateLayerIcon()
{
    QIcon icon;
    if (m_tabBar->count() > 0)
        icon = m_tabBar->tabIcon(m_tabBar->currentIndex());
    m_configToolButton->setIcon(icon);
}

void LayerBar::repopulateLayerTabs(DesignLayerSet *set)
{
    while (m_tabBar->count() > 0)
        m_tabBar->removeTab(0);
    foreach (DesignLayer *layer, set->allLayers()) {
        int tabIndex = m_tabBar->addTab(layer->name());
        m_tabBar->setTabData(tabIndex, QVariant::fromValue<DesignLayer *>(layer));
    }
}

void LayerBar::populateConfigMenu()
{
    m_configMenu->clear();

    m_configMenu->addSeparator();
    m_configMenu->addAction(m_showConfigDialogAction);

    m_colorMenu->addAction(m_showColorDialogAction);
    m_colorMenu->addSeparator();
    m_colorMenu->addActions(m_colorActionGroup->actions());
    m_configMenu->addMenu(m_colorMenu);

    m_setMenu->addAction(m_showSetDialogAction);
    m_setMenu->addSeparator();
    m_setMenu->addActions(m_setActionGroup->actions());
    m_configMenu->addMenu(m_setMenu);

    m_opacityMenu->addAction(m_showOpacityDialogAction);
    m_opacityMenu->addSeparator();
    m_opacityMenu->addActions(m_opacityActionGroup->actions());
    m_configMenu->addMenu(m_opacityMenu);

    m_configToolButton->setMenu(m_configMenu);
}

void LayerBar::disconnectTabBar()
{
    m_tabBar->disconnect(this);
}

void LayerBar::connectTabBar()
{
    connect(m_tabBar, &QTabBar::currentChanged,
            this, &LayerBar::activateLayer);
}

void LayerBar::createActions()
{
    m_activateNextAction = new QAction(QIcon::fromTheme("go-next"),
                                       QString("Activate next layer"), this);
    m_activateNextAction->setShortcut(QKeySequence("+"));
    m_activatePreviousAction = new QAction(QIcon::fromTheme("go-previous"),
                                           "Activate next layer", this);
    m_activatePreviousAction->setShortcut(QKeySequence("-"));
    m_activateNextSignalAction = new QAction(QIcon::fromTheme("go-next"),
                                             "Activate next signal layer", this);
    m_activateNextSignalAction->setShortcut(QKeySequence("/"));
    m_activatePreviousSignalAction = new QAction(QIcon::fromTheme("go-previous"),
                                                 "Activate next signal layer", this);
    m_activatePreviousSignalAction->setShortcut(QKeySequence("*"));

    m_showConfigDialogAction = new QAction(QIcon::fromTheme("preferences-system"),
                                           "View Configuration...", this);
    m_showConfigDialogAction->setShortcut(QKeySequence("shift+l"));
    m_showColorDialogAction = new QAction(QIcon::fromTheme("preferences-other"),
                                          "Color profiles...", this);
    m_showSetDialogAction = new QAction(QIcon::fromTheme("preferences-other"),
                                        "Layer sets...", this);
    m_showOpacityDialogAction = new QAction(QIcon::fromTheme("preferences-other"),
                                            "Opacity profiles...", this);

    m_colorActionGroup = new QActionGroup(this);
    m_setActionGroup  = new QActionGroup(this);
    m_opacityActionGroup = new QActionGroup(this);
}

void LayerBar::createMenus()
{
    m_configMenu = new QMenu("View configuration");
    m_colorMenu = new QMenu("Color profiles");
    m_setMenu = new QMenu("Layer sets");
    m_opacityMenu = new QMenu("Opacity profiles");
}

void LayerBar::connectActions()
{
    connect(m_activateNextAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
       activateNextLayer();
    });
    connect(m_activatePreviousAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
       activatePreviousLayer();
    });
    connect(m_activateNextSignalAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
       activateNextSignalLayer();
    });
    connect(m_activatePreviousSignalAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
       activatePreviousSignalLayer();
    });

    connect(m_showConfigDialogAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
        // TODO
    });
    connect(m_showColorDialogAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
        // TODO
    });
    connect(m_showSetDialogAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
        // TODO
    });
    connect(m_showOpacityDialogAction, &QAction::triggered,
            this, [this](bool checked) {
        Q_UNUSED(checked);
        // TODO
    });

    connect(m_colorActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        PcbPalette *palette = action->data().value<PcbPalette *>();
        m_paletteManager->setActivePalette(palette);
    });
    connect(m_setActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        DesignLayerSet *set = action->data().value<DesignLayerSet *>();
        setActiveLayerSet(set);
    });
    connect(m_opacityActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        Q_UNUSED(action);
        // TODO
    });
}

void LayerBar::disconnectActions()
{
    m_activateNextAction->disconnect(this);
    m_activatePreviousAction->disconnect(this);
    m_activateNextSignalAction->disconnect(this);
    m_activatePreviousSignalAction->disconnect(this);
}


//ColorProfileEditor *dlg = new ColorProfileEditor(this);
//dlg->setWindowFlags(Qt::Dialog);
//dlg->initialise();
//dlg->show();
