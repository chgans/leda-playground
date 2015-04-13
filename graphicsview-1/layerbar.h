#ifndef LAYERBAR_H
#define LAYERBAR_H

#include <QWidget>

class QTabBar;
class QAction;
class QActionGroup;
class QToolButton;

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

    void onActivePaletteChanged(PcbPalette *palette);
    void onPaletteChanged(PcbPalette *palette);
    void onPaletteAdded(PcbPalette *palette);
    void onPaletteRemoved(PcbPalette *palette);

    void onActiveLayerSetChanged(DesignLayerSet *set);
    void onLayerSetChanged(DesignLayerSet *set);
    void onLayerSetAdded(DesignLayerSet *set);
    void onLayerSetRemoved(DesignLayerSet *set);

private:
    // Profile and conf
    PcbPaletteManager *m_paletteManager;
    DesignLayerManager *m_layerManager;
    void connectPaletteManager();
    void disconnectPaletteManager();
    void connectLayerManager();
    void disconnectLayerManager();

    // Widgets
    QTabBar *m_tabBar;
    QToolButton *m_layerToolButton;
    void createTabBar();
    void createLayerToolButton();
    void updateTabIcons();
    void updateLayerIcon();
    void repopulateLayerTabs();
    void disconnectTabBar();
    void connectTabBar();

    // Actions
    void createActions();
    void connectActions();
    void disconnectActions();
    QAction *m_activateNextAction;
    QAction *m_activatePreviousAction;
    QAction *m_activateNextSignalAction;
    QAction *m_activatePreviousSignalAction;
    QActionGroup *m_layerColorActionGroup;
    QList<QAction *> m_layerColorActions;
    QActionGroup *m_layerSetActionGroup;
    QList<QAction *> m_layerSetActions;
    QActionGroup *m_layerOpacityActionGroup;
    QList<QAction *> m_layerOpacityActions;

};

#endif // LAYERBAR_H
