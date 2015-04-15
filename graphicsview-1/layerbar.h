#ifndef LAYERBAR_H
#define LAYERBAR_H

#include <QWidget>

class QTabBar;
class QAction;
class QActionGroup;
class QToolButton;
class QMenu;

class PcbPalette;
class PcbPaletteManager;
class DesignLayer;
class DesignLayerSet;
class DesignLayerManager;

class LayerBar : public QWidget
{
    Q_OBJECT
public:
    explicit LayerBar(QWidget *parent = 0);
    ~LayerBar();

signals:

public slots:

protected slots:
    void activateLayer(int tabIndex);
    void activateNextLayer();
    void activatePreviousLayer();
    void activateNextSignalLayer();
    void activatePreviousSignalLayer();

    void setActivePalette(PcbPalette *palette);
    void onPaletteChanged(PcbPalette *palette);
    void addPalette(PcbPalette *palette);
    void removePalette(PcbPalette *palette);

    void setActiveLayerSet(DesignLayerSet *set);
    void onLayerSetChanged(DesignLayerSet *set);
    void addLaterSet(DesignLayerSet *set);
    void removeLayerSet(DesignLayerSet *set);

private:
    // Profile and conf
    PcbPaletteManager *m_paletteManager;
    DesignLayerManager *m_layerManager;
    DesignLayerSet *m_activeLayerSet;
    void connectPaletteManager();
    void disconnectPaletteManager();
    void connectLayerManager();
    void disconnectLayerManager();

    // Widgets
    QTabBar *m_tabBar;
    QToolButton *m_configToolButton;
    void createTabBar();
    void createConfigToolButton();
    void updateTabIcons();
    void updateLayerIcon();
    void repopulateLayerTabs(DesignLayerSet *set);
    void populateConfigMenu();
    void disconnectTabBar();
    void connectTabBar();

    // Actions and menus
    void createActions();
    void createMenus();
    void connectActions();
    void disconnectActions();
    QAction *m_activateNextAction;
    QAction *m_activatePreviousAction;
    QAction *m_activateNextSignalAction;
    QAction *m_activatePreviousSignalAction;
    QMenu *m_configMenu;
    QAction *m_showConfigDialogAction;
    QMenu *m_colorMenu;
    QAction *m_showColorDialogAction;
    QActionGroup *m_colorActionGroup;
    QMenu *m_setMenu;
    QAction *m_showSetDialogAction;
    QActionGroup *m_setActionGroup;
    QMenu *m_opacityMenu;
    QAction *m_showOpacityDialogAction;
    QActionGroup *m_opacityActionGroup;
    QMenu *m_tabMenu;
};

#endif // LAYERBAR_H
