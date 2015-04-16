#include "layerbar.h"

#include "mainview.h"
#include "scene.h"
#include "designlayer.h"
#include "designlayerset.h"
#include "pcbpalette.h"

#include <QAction>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QTabBar>
#include <QToolButton>
#include <QMenu>
#include <QKeySequence>

#include <QDebug>

/*
 * TODO:
 * - layer enabled vs visible
 * - flipped view eg. view->setOrientation(Le::TopFace)
 */

static QIcon createColorIcon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

LayerBar::LayerBar(QWidget *parent) : QWidget(parent)
{
    m_view = nullptr;

    createConfigToolButton();
    createTabBar();
    createActions();
    createMenus();

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_configToolButton);
    mainLayout->addWidget(m_tabBar);
    setLayout(mainLayout);

    populateConfigMenu();

    connectActions();
    connectTabBar();
}

LayerBar::~LayerBar()
{
}

void LayerBar::setView(MainView *view)
{
    m_view = view;

    connect(view, &MainView::layerAdded,
            this, &LayerBar::addLayerTab);
    connect(view, &MainView::layerRemoved,
            this, &LayerBar::removeLayerTab);

    updateLayerTabs();
    updateTabIcons();
    updateLayerIcon();
}

void LayerBar::activateLayer(int tabIndex)
{
    DesignLayer *layer = m_tabBar->tabData(tabIndex).value<DesignLayer *>();
    qDebug() << "Activating layer" << layer->index() << layer->defaultName();
    if (m_view != nullptr)
        m_view->setActiveLayer(layer);
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
    Q_ASSERT(palette);

    if (m_activePalette == palette)
        return;

    m_activePalette = palette;

    updateTabIcons();
    updateLayerIcon();

    foreach (QAction *action, m_colorActionGroup->actions()) {
        qDebug() << action->data().value<PcbPalette *>();
        if (action->data().value<PcbPalette *>() == palette) {
            action ->setChecked(true);
            return;
        }
    }
    Q_ASSERT(false);
}

void LayerBar::onPaletteChanged(PcbPalette *palette)
{
    updateTabIcons();
    updateLayerIcon();
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

void LayerBar::onLayerSetChanged(DesignLayerSet *set)
{
    Q_UNUSED(set);
    disconnectTabBar();
    updateLayerTabs();
    updateTabIcons();
    updateLayerIcon();
    connectTabBar();
}

void LayerBar::addLaterSet(DesignLayerSet *set)
{
    QAction *action = new QAction(set->effectiveName(), this);
    action->setData(QVariant::fromValue<DesignLayerSet *>(set));
    m_setActionGroup->addAction(action);
    m_setMenu->addAction(action);

    // TODO: monitor set changes here
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

void LayerBar::addLayer(DesignLayer *layer)
{
    Q_ASSERT(!m_availableLayers.contains(layer));
    m_availableLayers.append(layer);
    if (layer->isPresent()) {
        m_view->addLayer(layer);
    }
}

void LayerBar::removeLayer(DesignLayer *layer)
{
    Q_ASSERT(m_availableLayers.contains(layer));
    m_availableLayers.removeOne(layer);
    if (m_view) {
        if (m_view->activeLayer() == layer && m_availableLayers.count() > 0)
            m_view->setActiveLayer(m_availableLayers.first());
        m_view->removeLayer(layer);
    }
}

void LayerBar::addLayerTab(DesignLayer *layer)
{
    disconnectTabBar();
    int tabIndex = m_tabBar->addTab(layer->defaultName());
    m_tabBar->setTabData(tabIndex, QVariant::fromValue<DesignLayer *>(layer));
    m_tabBar->setTabIcon(tabIndex, createColorIcon(layer->color()));
    updateLayerIcon();
    if (layer == m_view->activeLayer())
        m_tabBar->setCurrentIndex(tabIndex);
    connectTabBar();
}

void LayerBar::removeLayerTab(DesignLayer *layer)
{
    disconnectTabBar();
    for (int i = 0; i < m_tabBar->count(); i++) {
        DesignLayer *tabLayer = m_tabBar->tabData(i).value<DesignLayer *>();
        if (layer == tabLayer) {
            m_tabBar->removeTab(i);
            break;
        }
    }
    connectTabBar();
}

void LayerBar::createTabBar()
{
    m_tabBar = new QTabBar();
    m_tabBar->setShape(QTabBar::RoundedSouth);
    m_tabBar->setDrawBase(false);
    m_tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
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
    for (int tabIndex = 0; tabIndex < m_tabBar->count(); tabIndex++) {
        DesignLayer *layer = m_tabBar->tabData(tabIndex).value<DesignLayer *>();
        QColor color = layer->color();
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

void LayerBar::updateLayerTabs()
{
    while (m_tabBar->count() > 0)
        m_tabBar->removeTab(0);

    if (m_view == nullptr)
        return;

    int currentIndex = -1;
    foreach (DesignLayer *layer, m_view->layers()) {
        int tabIndex = m_tabBar->addTab(layer->defaultName());
        m_tabBar->setTabData(tabIndex, QVariant::fromValue<DesignLayer *>(layer));
        if (layer == m_view->activeLayer())
            currentIndex = tabIndex;
    }
    if (currentIndex != -1)
        m_tabBar->setCurrentIndex(currentIndex);
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
    connect(m_tabBar, &QTabBar::customContextMenuRequested,
            this, &LayerBar::showTabContextMenu);
}

void LayerBar::showTabContextMenu(const QPoint &pos)
{
    if (m_tabBar->count() == 0 || m_view == nullptr)
        return;

    int tabIndex = m_tabBar->tabAt(pos);
    DesignLayer *layer = m_tabBar->tabData(tabIndex).value<DesignLayer *>();
    QMenu *menu = new QMenu("Layer tab contextual menu");
    QAction *action;
    action = new QAction(createColorIcon(layer->color()),
                         QString("Hide %1").arg(layer->defaultName()), menu);
    connect(action, &QAction::triggered,
            this, [this, layer](bool checked) {
        Q_UNUSED(checked);
        m_view->removeLayer(layer);
    });
    menu->addAction(action);

    menu->addSeparator();
    QMenu *hideMenu = new QMenu("Hide layers");
    menu->addMenu(hideMenu);
    QMenu *showMenu = new QMenu("Show layers");
    menu->addMenu(showMenu);

    foreach (DesignLayer *layer, m_availableLayers) {
        qDebug() << __PRETTY_FUNCTION__ << layer->index()
                 << layer->isEnabled() << layer->isPresent() << layer->isVisible();
        if (m_view->layers().contains(layer)) {
            action = new QAction(createColorIcon(layer->color()),
                                 QString("Hide %1").arg(layer->defaultName()), menu);
            connect(action, &QAction::triggered,
                    this, [this, layer](bool checked) {
                Q_UNUSED(checked);
                m_view->removeLayer(layer);
            });
            hideMenu->addAction(action);
        }
        else {
            action = new QAction(createColorIcon(layer->color()),
                                 QString("Show %1").arg(layer->defaultName()), menu);
            connect(action, &QAction::triggered,
                    this, [this, layer](bool checked) {
                Q_UNUSED(checked);
                m_view->addLayer(layer);
            });
            showMenu->addAction(action);
        }
    }

    // Layer sets
    menu->addMenu(m_setMenu);

    // TODO: not implemented
    menu->addSeparator();
    QActionGroup *group = new QActionGroup(menu);
    action = new QAction("Use short layer names", group);
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);
    action = new QAction("Use medium layer names", group);
    action->setCheckable(true);
    menu->addAction(action);
    action = new QAction("Use long layer names", group);
    action->setCheckable(true);
    menu->addAction(action);

    // TODO: not implemented
    menu->addSeparator();
    action = new QAction("Flipped", menu);
    action->setCheckable(true);
    menu->addAction(action);

    menu->exec(m_tabBar->mapToGlobal(pos));
    delete menu;
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
        m_view->setPalette(palette);
        updateLayerIcon();
        updateTabIcons();
    });
    connect(m_setActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        DesignLayerSet *set = action->data().value<DesignLayerSet *>();
        if (m_view != nullptr) {
            m_view->removeLayers(m_view->layers());
            m_view->addLayers(set->enabledLayers());
        }
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
