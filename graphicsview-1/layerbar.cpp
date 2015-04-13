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

    createLayerToolButton();
    createTabBar();
    createActions();

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_layerToolButton);
    mainLayout->addWidget(m_tabBar);
    setLayout(mainLayout);

    repopulateLayerTabs();
    updateTabIcons();
    updateLayerIcon();

    connectActions();
    connectTabBar();
    connectLayerManager();
    connectPaletteManager();
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

void LayerBar::onActivePaletteChanged(PcbPalette *palette)
{

}

void LayerBar::onPaletteChanged(PcbPalette *palette)
{

}

void LayerBar::onPaletteAdded(PcbPalette *palette)
{

}

void LayerBar::onPaletteRemoved(PcbPalette *palette)
{

}

void LayerBar::onActiveLayerSetChanged(DesignLayerSet *set)
{
    disconnectTabBar();
    repopulateLayerTabs();
    updateTabIcons();
    updateLayerIcon();
    connectTabBar();
}

void LayerBar::onLayerSetChanged(DesignLayerSet *set)
{
    disconnectTabBar();
    repopulateLayerTabs();
    updateTabIcons();
    updateLayerIcon();
    connectTabBar();
}

void LayerBar::onLayerSetAdded(DesignLayerSet *set)
{

}

void LayerBar::onLayerSetRemoved(DesignLayerSet *set)
{

}

void LayerBar::connectPaletteManager()
{
    connect(m_paletteManager, &PcbPaletteManager::paletteChanged,
            this, &LayerBar::onPaletteChanged);
    connect(m_paletteManager, &PcbPaletteManager::paletteAdded,
            this, &LayerBar::onPaletteAdded);
    connect(m_paletteManager, &PcbPaletteManager::paletteRemoved,
            this, &LayerBar::onPaletteRemoved);
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
            this, &LayerBar::onLayerSetAdded);
    connect(m_layerManager, &DesignLayerManager::layerSetRemoved,
            this, &LayerBar::onLayerSetRemoved);
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

void LayerBar::createLayerToolButton()
{
    m_layerToolButton = new QToolButton;
    m_layerToolButton->setAutoRaise(true);
    m_layerToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
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
    m_layerToolButton->setIcon(icon);
}

void LayerBar::repopulateLayerTabs()
{
    while (m_tabBar->count())
        m_tabBar->removeTab(0);
    foreach (DesignLayer *layer, m_layerManager->enabledLayers()) {
        int tabIndex = m_tabBar->addTab(layer->name());
        m_tabBar->setTabData(tabIndex, QVariant::fromValue<DesignLayer *>(layer));
    }
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
                                       QString("Activate next layer"),
                                       this);
    m_activatePreviousAction = new QAction(QIcon::fromTheme("go-previous"),
                                           "Activate next layer",
                                           this);
    m_activateNextSignalAction = new QAction(QIcon::fromTheme("go-next"),
                                             "Activate next signal layer",
                                             this);
    m_activatePreviousSignalAction = new QAction(QIcon::fromTheme("go-previous"),
                                                 "Activate next signal layer",
                                                 this);

    m_layerColorActionGroup = new QActionGroup(this);
    m_layerSetActionGroup  = new QActionGroup(this);
    m_layerOpacityActionGroup = new QActionGroup(this);
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
}

void LayerBar::disconnectActions()
{
    m_activateNextAction->disconnect(this);
    m_activatePreviousAction->disconnect(this);
    m_activateNextSignalAction->disconnect(this);
    m_activatePreviousSignalAction->disconnect(this);
}

