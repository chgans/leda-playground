#include "pcbeditorwidget.h"
#include "mainview.h"
#include "scene.h"
#include "designlayer.h"
#include "designlayermanager.h"
#include "pcbpalette.h"
#include "pcbpalettemanager.h"
#include "colorprofilecombobox.h"
#include "colorprofiletoolbutton.h"
#include "colorprofileeditor.h"
#include "layerbar.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabBar>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>
#include <QWheelEvent>

#include <QDebug>

static QIcon icon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

PcbEditorWidget::PcbEditorWidget(QWidget *parent) :
    QWidget(parent)
{
    m_paletteManager = PcbPaletteManager::instance();
    m_layerManager = DesignLayerManager::instance();

    m_layerManager = DesignLayerManager::instance();

    m_layerBar = new LayerBar;

    mView = new MainView();
    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#if 0
    mCurrentLayerButton = new QToolButton;
    mCurrentLayerButton->setAutoRaise(true);
    mCurrentLayerButton->setIcon(m_layerBar->tabIcon(0));
    mCurrentLayerButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // TODO: menu to pick up a profile + edit profiles
    //QAction *editColor = new QAction(this);
    connect(mCurrentLayerButton, SIGNAL(clicked()),
            this, SLOT(showColorDialog()));
    //mCurrentLayerButton->addAction(editColor);
#endif

#if 1
    mLayerSetButton = new QToolButton;
    mLayerSetButton->setText("LS ");
    mLayerSetButton->setAutoRaise(true);
    mLayerSetButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    QMenu *menu = new QMenu(this);
    QMenu *submenu = new QMenu(this);
    QActionGroup *actionGroup;
    QAction *action;

    actionGroup = new QActionGroup(this);
    menu->addAction("View configuration...");

    actionGroup = new QActionGroup(this);
    submenu = menu->addMenu("Color profiles");
    submenu->addAction("Configuration...");
    submenu->addSeparator();
    actionGroup->setExclusive(true);
    foreach (PcbPalette *palette, PcbPaletteManager::instance()->palettes()) {
        action = actionGroup->addAction(palette->name());
        action->setData(QVariant::fromValue<PcbPalette *>(palette));
        action->setCheckable(true);
        submenu->addAction(action);
    }
    connect(actionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        PcbPalette *palette = action->data().value<PcbPalette *>();
        PcbPaletteManager::instance()->setActivePalette(palette);
    });
    actionGroup = new QActionGroup(this);
    submenu = menu->addMenu("Opacity profiles");
    submenu->addAction("Configuration...");
    submenu->addSeparator();
    actionGroup->setExclusive(true);/*
    foreach (PcbPalette *palette, PcbPaletteManager::instance()->palettes()) {
        action = actionGroup->addAction(palette->name());
        action->setData(QVariant::fromValue<PcbPalette *>(palette));
        action->setCheckable(true);
        submenu->addAction(action);
    }*/
    actionGroup = new QActionGroup(this);
    submenu = menu->addMenu("Layer sets...");
    submenu->addAction("Configuration...");
    submenu->addSeparator();
    // TODO: trigger layer set dialog
    actionGroup->setExclusive(true);
    foreach (DesignLayerSet *set, DesignLayerManager::instance()->allLayerSets()) {
        action = actionGroup->addAction(set->effectiveName());
        action->setData(QVariant::fromValue<DesignLayerSet *>(set));
        action->setCheckable(true);
        submenu->addAction(action);
    }

    mLayerSetButton->setMenu(menu);
    mLayerSetButton->setPopupMode(QToolButton::InstantPopup);
#endif

    mSnapButton = new QToolButton;
    mSnapButton->setText("Snap");
    mSnapButton->setAutoRaise(true);
    mSnapButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    mMaskButton = new QToolButton;
    mMaskButton->setText("Mask");
    mMaskButton->setAutoRaise(true);
    mMaskButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    mClearButton = new QToolButton;
    mClearButton->setText("Clear");
    mClearButton->setAutoRaise(true);
    mClearButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    QHBoxLayout *toolLayout = new QHBoxLayout;
//    toolLayout->addWidget(mCurrentLayerButton);
    toolLayout->addWidget(mLayerSetButton);
    toolLayout->addWidget(m_layerBar);
//    toolLayout->addWidget(mSnapButton);
//    toolLayout->addWidget(mMaskButton);
//    toolLayout->addWidget(mClearButton);
    toolLayout->addWidget(new ColorProfileComboBox);
    toolLayout->addWidget(new ColorProfileToolButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mView);
    mainLayout->addLayout(toolLayout);
    mainLayout->setMargin(0);

    setLayout(mainLayout);

    createActions();
    createBoardInsightMenu();
}

PcbEditorWidget::~PcbEditorWidget()
{

}

void PcbEditorWidget::setScene(Scene *scene)
{
    mView->setScene(scene);
    mView->scale(0.75, 0.75);
    //setupLayerTabBar();
    //onColorProfileChanged(m_paletteManager->activePalette());
}

void PcbEditorWidget::activateEditor(QMainWindow *window)
{
    window->addAction(mShowBoardInsightPopUpMenuAction);
    window->addAction(mToggleHeadsUpDisplayAction);
    window->addAction(mToggleHeadsUpTrackingAction);
    window->addAction(mResetHeadsUpDeltaOriginAction);
    window->addAction(mToggleHeadsUpDeltaOriginAction);
    window->addAction(mToggleInsightLensAction);
    window->addAction(mToggleInsightLensShapeAction);
    window->addAction(mToggleInsightLensTrackingAction);
    window->addAction(mShiftInsightLensToMouseAction);
    window->addAction(mToggleInsightLensAutoZoomAction);
    window->addAction(mToggleInsightLensSingleLayerModeAction);
    window->addAction(m_cycleDisplayLayerMode);
}

void PcbEditorWidget::wheelEvent(QWheelEvent *event)
{
    // FIXME: doesn't work here, wheel events on MainView
    // Move activate/previous/next layer in view?
//    if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) {
//        if (event->delta() > 0)
//            m_layerBar->activateNextLayer();
//        else
//            m_layerBar->activatePreviousLayer();
//    }
}

void PcbEditorWidget::createActions()
{
    /*
     *  2D Board Insight System
     */

    mShowBoardInsightPopUpMenuAction = new QAction("Show Board Insight Pop-Up Menu", this);
    mShowBoardInsightPopUpMenuAction->setShortcut(QKeySequence(Qt::Key_F2));
    connect(mShowBoardInsightPopUpMenuAction, SIGNAL(triggered()),
            this, SLOT(showBoardInsightPopUpMenu()));

    mToggleHeadsUpDisplayAction = new QAction("Toggle Heads Up Display", this);
    mToggleHeadsUpDisplayAction->setShortcut(QKeySequence("Shift+H"));
    mToggleHeadsUpDisplayAction->setCheckable(true);
    mToggleHeadsUpDisplayAction->setChecked(mView->headsUpEnabled());
    connect(mToggleHeadsUpDisplayAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableHeadsUp(bool)));

    mToggleHeadsUpTrackingAction = new QAction("Toggle Heads Up Tracking", this);
    mToggleHeadsUpTrackingAction->setShortcut(QKeySequence("Shift+G"));
    mToggleHeadsUpTrackingAction->setCheckable(true);
    mToggleHeadsUpTrackingAction->setChecked(mView->headsUpTrackingEnabled());
    connect(mToggleHeadsUpTrackingAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableHeadsUpTracking(bool)));

    mResetHeadsUpDeltaOriginAction = new QAction("Reset Heads Up Delta Origin", this);
    mResetHeadsUpDeltaOriginAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(mResetHeadsUpDeltaOriginAction, SIGNAL(triggered()),
            mView, SLOT(resetHeadsUpDeltaOrigin()));

    mToggleHeadsUpDeltaOriginAction = new QAction("Toggle Heads Up Delta Origin", this);
    mToggleHeadsUpDeltaOriginAction->setShortcut(QKeySequence("Shit+D"));
    mToggleHeadsUpDeltaOriginAction->setCheckable(true);
    mToggleHeadsUpDeltaOriginAction->setChecked(mView->headsUpDeltaOriginEnabled());
    connect(mToggleHeadsUpDeltaOriginAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableHeadsUpDeltaOrigin(bool)));

    mToggleInsightLensAction = new QAction("Toggle Insight Lens", this);
    mToggleInsightLensAction->setShortcut(QKeySequence("Shift+M"));
    mToggleInsightLensAction->setCheckable(true);
    mToggleInsightLensAction->setChecked(mView->insightLensEnabled());
    connect(mToggleInsightLensAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableInsightLens(bool)));

    mToggleInsightLensShapeAction = new QAction("Toggle Insight Lens shape", this);
    mToggleInsightLensShapeAction->setShortcut(QKeySequence("Shift+B"));
    connect(mToggleInsightLensShapeAction, SIGNAL(triggered()),
            mView, SLOT(toggleInsightLensShape()));

    mShiftInsightLensToMouseAction = new QAction("Shift Insight Lens To Mouse", this);
    mShiftInsightLensToMouseAction->setShortcut(QKeySequence("Shift+Ctrl+N"));
    connect(mShiftInsightLensToMouseAction, SIGNAL(triggered()),
            mView, SLOT(shiftInsightLensToMouse()));

    mToggleInsightLensTrackingAction = new QAction("Toggle Insight Lens Tracking", this);
    mToggleInsightLensTrackingAction->setShortcut(QKeySequence("Shift+N"));
    mToggleInsightLensTrackingAction->setCheckable(true);
    mToggleInsightLensTrackingAction->setChecked(mView->insightLensMouseTrackingEnabled());
    connect(mToggleInsightLensTrackingAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableInsightLensTracking(bool)));

    mToggleInsightLensAutoZoomAction = new QAction("Toggle Insight Lens Auto Zoom", this);
    mToggleInsightLensAutoZoomAction->setShortcut(QKeySequence("Shift+Ctrl+M"));
    mToggleInsightLensAutoZoomAction->setCheckable(true);
    mToggleInsightLensAutoZoomAction->setChecked(mView->insightLensAutoZoomEnabled());
    connect(mToggleInsightLensAutoZoomAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableInsightLensAutoZoom(bool)));

    mToggleInsightLensSingleLayerModeAction = new QAction("Toggle Insight Lens Single Layer Mode", this);
    mToggleInsightLensSingleLayerModeAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    mToggleInsightLensSingleLayerModeAction->setCheckable(true);
    mToggleInsightLensSingleLayerModeAction->setChecked(mView->insightLensSingleLayerEnabled());
    connect(mToggleInsightLensSingleLayerModeAction, SIGNAL(triggered(bool)),
            mView, SLOT(enableInsightLensSingleLayerMode(bool)));

    m_cycleDisplayLayerMode = new QAction("Cycle Single Layer Mode", this);
    m_cycleDisplayLayerMode->setShortcut(QKeySequence("Shift+S"));
    connect(m_cycleDisplayLayerMode, SIGNAL(triggered()),
            mView, SLOT(cycleLayerDisplayMode()));
#if 0
    mInsightLensZoomInAction; // Alt+WheelUp
    mInsightLensZoomOutAction; // Alt+WheelDown

    mShowInsightSystemViolationAction; // Shift+X
    mShowInsightSystemOtherAction; // Shift+V
    mCycleSingleLayerModeAction; // Shift+S

    mActivateNextLayerAction; // Ctrl+Shfit+WheelUp or KPD+
    mActivatePreviousLayerAction; // // Ctrl+Shfit+WheelDown or KPD-
    mActivateNextSignalLayerAction; // KPD*
    mActivatePreviousSignalLayerAction; // // KPD/

    mShowViewConfigurationDialogAction; // L
#endif
}

void PcbEditorWidget::createBoardInsightMenu()
{
    mBoardInsightPopUpMenu = new QMenu("Board Insight Pop-Up Menu", this);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpDisplayAction);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpTrackingAction);
    mBoardInsightPopUpMenu->addAction(mResetHeadsUpDeltaOriginAction);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpDeltaOriginAction);
    mBoardInsightPopUpMenu->addSeparator();
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensShapeAction);
    mBoardInsightPopUpMenu->addAction(mShiftInsightLensToMouseAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensTrackingAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensAutoZoomAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensSingleLayerModeAction);
}

void PcbEditorWidget::createViewConfigurationMenu()
{

}

void PcbEditorWidget::createLayerTabBar()
{

}

Scene *PcbEditorWidget::scene() const
{
    Q_ASSERT(mView && mView->scene());
    return static_cast<Scene *>(mView->scene());
}

void PcbEditorWidget::showColorDialog()
{
    ColorProfileEditor *dlg = new ColorProfileEditor(this);
    dlg->setWindowFlags(Qt::Dialog);
    dlg->initialise();
    dlg->show();
}

void PcbEditorWidget::showBoardInsightPopUpMenu()
{
    mBoardInsightPopUpMenu->exec(QCursor::pos());
}
