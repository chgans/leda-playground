#ifndef LAYERBAR_H
#define LAYERBAR_H

#include <QWidget>

class QTabBar;
class QAction;
class QActionGroup;
class QToolButton;
class QMenu;

class MainView;
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

    void setView(MainView *view);
    MainView *view() const;

signals:

public slots:
    void addPalette(PcbPalette *palette);
    void removePalette(PcbPalette *palette);
    void setActivePalette(PcbPalette *palette);
    void addLaterSet(DesignLayerSet *set);
    void removeLayerSet(DesignLayerSet *set);
    void addLayer(DesignLayer *layer);
    void removeLayer(DesignLayer *layer);

protected slots:
    void activateLayer(int tabIndex);
    void activateNextLayer();
    void activatePreviousLayer();
    void activateNextSignalLayer();
    void activatePreviousSignalLayer();

    void onPaletteChanged(PcbPalette *palette);

    void onLayerSetChanged(DesignLayerSet *set);

    void addLayerTab(DesignLayer *layer);
    void removeLayerTab(DesignLayer *layer);

private:
    MainView *m_view;
    QList<DesignLayer *> m_availableLayers;

    // Profile and conf
    PcbPalette *m_activePalette;

    // Widgets
    QTabBar *m_tabBar;
    QToolButton *m_configToolButton;
    void createTabBar();
    void createConfigToolButton();
    void updateTabIcons();
    void updateLayerIcon();
    void updateLayerTabs();
    void populateConfigMenu();
    void disconnectTabBar();
    void connectTabBar();
    void showTabContextMenu(const QPoint &pos);

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
};

#endif // LAYERBAR_H
