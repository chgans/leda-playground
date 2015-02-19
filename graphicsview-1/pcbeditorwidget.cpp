#include "pcbeditorwidget.h"
#include "mainview.h"
#include "pcbpalettemanagerdialog.h"

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

static QIcon *icon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return new QIcon(pix);
}

PcbEditorWidget::PcbEditorWidget(QWidget *parent) :
    QWidget(parent)
{
    mTabBar = new QTabBar;
    mTabBar->setShape(QTabBar::RoundedSouth);
    mTabBar->setDrawBase(false);
    mTabBar->addTab("Top layer");
    mTabBar->setTabIcon(0, *icon(QColor("#ff0000")));
    mTabBar->addTab("Bottom layer");
    mTabBar->setTabIcon(1, *icon(QColor("#0000ff")));
    mTabBar->addTab("Mechanical 1");
    mTabBar->setTabIcon(2, *icon(QColor("#ff00ff")));
    mTabBar->addTab("Mechanical 13");
    mTabBar->setTabIcon(3, *icon(QColor("#ff00ff")));
    mTabBar->addTab("Mechanical 15");
    mTabBar->setTabIcon(4, *icon(QColor("#008000")));
    mTabBar->addTab("Top overlay");
    mTabBar->setTabIcon(5, *icon(QColor("#ffff00")));
    mTabBar->addTab("Bottom overlay");
    mTabBar->setTabIcon(6, *icon(QColor("#808000")));
    mTabBar->addTab("Top paste");
    mTabBar->setTabIcon(7, *icon(QColor("#808080")));
    mTabBar->addTab("Bottom paste");
    mTabBar->setTabIcon(8, *icon(QColor("#800000")));
    mTabBar->addTab("Top solder");
    mTabBar->setTabIcon(9, *icon(QColor("#800080")));
    mTabBar->addTab("Bottom solder");
    mTabBar->setTabIcon(10, *icon(QColor("#ff00ff")));
    mTabBar->addTab("Drill guide");
    mTabBar->setTabIcon(11, *icon(QColor("#800000")));
    mTabBar->addTab("Keep-Out layer");
    mTabBar->setTabIcon(12, *icon(QColor("#ff00ff")));
    mTabBar->addTab("Drill drawing");
    mTabBar->setTabIcon(13, *icon(QColor("#ff002a")));
    mTabBar->addTab("Multi-Layer");
    mTabBar->setTabIcon(14, *icon(QColor("#c0c0c0")));
    connect(mTabBar, SIGNAL(currentChanged(int)),
            this, SLOT(activateLayer(int)));

    mView = new MainView();
    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    mCurrentLayerButton = new QToolButton;
    mCurrentLayerButton->setAutoRaise(true);
    mCurrentLayerButton->setIcon(mTabBar->tabIcon(0));
    mCurrentLayerButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // TODO: menu to pick up a profile + edit profiles
    //QAction *editColor = new QAction(this);
    connect(mCurrentLayerButton, SIGNAL(clicked()),
            this, SLOT(showColorDialog()));
    //mCurrentLayerButton->addAction(editColor);

    mLayerSetButton = new QToolButton;
    mLayerSetButton->setText("LS ");
    mLayerSetButton->setAutoRaise(true);
    mLayerSetButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    QMenu *menu = new QMenu(this);
    menu->addAction("Board layer sets...");
    menu->addSeparator();
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    QAction *action = actionGroup->addAction("All layers");
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);
    action = actionGroup->addAction("Signal layers");
    action->setCheckable(true);
    menu->addAction(action);
    action = actionGroup->addAction("Plane layers");
    action->setCheckable(true);
    menu->addAction(action);
    action = actionGroup->addAction("Non-Signal layers");
    action->setCheckable(true);
    menu->addAction(action);
    action = actionGroup->addAction("Mechanical layers");
    action->setCheckable(true);
    menu->addAction(action);
    mLayerSetButton->setMenu(menu);
    mLayerSetButton->setPopupMode(QToolButton::InstantPopup);

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
    toolLayout->addWidget(mCurrentLayerButton);
    toolLayout->addWidget(mLayerSetButton);
    toolLayout->addWidget(mTabBar);
    toolLayout->addWidget(mSnapButton);
    toolLayout->addWidget(mMaskButton);
    toolLayout->addWidget(mClearButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mView);
    mainLayout->addLayout(toolLayout);
    mainLayout->setMargin(0);

    setLayout(mainLayout);

    createActions();
    createMenus();
}

void PcbEditorWidget::wheelEvent(QWheelEvent *event)
{
    // FIXME: doesn't work here, wheel events on MainView
    // Move activate/previous/next layer in view?
    if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) {
        if (event->delta() > 0)
            activateNextLayer();
        else
            activatePreviousLayer();
    }
}

void PcbEditorWidget::activateLayer(int index)
{
    mCurrentLayerButton->setIcon(mTabBar->tabIcon(index));
}

void PcbEditorWidget::activateNextLayer()
{
    activateLayer((mTabBar->currentIndex() + 1 ) % mTabBar->count());
}

void PcbEditorWidget::activatePreviousLayer()
{
    activateLayer((mTabBar->currentIndex() - 1 ) % mTabBar->count());
}

void PcbEditorWidget::activateNextSignalLayer()
{
    activateNextLayer();
}

void PcbEditorWidget::activatePreviousSignalLayer()
{
    activatePreviousLayer();
}

void PcbEditorWidget::createActions()
{
    // 2D Board Insight System
    mToggleHeadsUpDisplayAction = new QAction("Toggle Heads Up Display", this);
    addAction(mToggleHeadsUpDisplayAction);
    mToggleHeadsUpDisplayAction->setShortcut(QKeySequence("Shift+H"));
    mToggleHeadsUpDisplayAction->setCheckable(true);
    connect(mToggleHeadsUpDisplayAction, SIGNAL(triggered(bool)),
            this, SLOT(enableHeadsUp(bool)));
    mToggleHeadsUpTrackingAction = new QAction("Toggle Heads Up Tracking", this);
    addAction(mToggleHeadsUpTrackingAction);
    mToggleHeadsUpTrackingAction->setShortcut(QKeySequence("Shift+G"));
    mToggleHeadsUpTrackingAction->setCheckable(true);
    connect(mToggleHeadsUpTrackingAction, SIGNAL(triggered(bool)),
            this, SLOT(enableHeadsUpTracking(bool)));
    mResetHeadsUpDeltaOriginAction = new QAction("Reset Heads Up Delta Origin", this);
    addAction(mResetHeadsUpDeltaOriginAction);
    mResetHeadsUpDeltaOriginAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(mResetHeadsUpDeltaOriginAction, SIGNAL(triggered()),
            SLOT(resetHeadsUpDeltaOrigin()));
    mToggleHeadsUpDeltaOriginAction = new QAction("Toggle Heads Up Delta Origin", this);
    addAction(mToggleHeadsUpDeltaOriginAction);
    mToggleHeadsUpDeltaOriginAction->setShortcut(QKeySequence("Shit+D"));
    mToggleHeadsUpDeltaOriginAction->setCheckable(true);
    connect(mToggleHeadsUpDeltaOriginAction, SIGNAL(triggered(bool)),
            this, SLOT(enableHeadsUpDeltaOrigin(bool)));
    mToggleInsightLensAction = new QAction("Toggle Insight Lens", this);
    addAction(mToggleInsightLensAction);
    mToggleInsightLensAction->setShortcut(QKeySequence("Shift+M"));
    mToggleInsightLensAction->setCheckable(true);
    connect(mToggleInsightLensAction, SIGNAL(triggered(bool)),
            this, SLOT(enableInsightLens(bool)));
    mShiftInsightLensToMouseAction = new QAction("Shift Insight Lens To Mouse", this);
    addAction(mShiftInsightLensToMouseAction);
    mShiftInsightLensToMouseAction->setShortcut(QKeySequence("Shift+Ctrl+N"));
    connect(mShiftInsightLensToMouseAction, SIGNAL(triggered()),
            this, SLOT(shiftInsightLensToMouse()));
    mToggleInsightLensTrackingAction = new QAction("Toggle Insight Lens Tracking", this);
    addAction(mToggleInsightLensTrackingAction);
    mToggleInsightLensTrackingAction->setShortcut(QKeySequence("Shift+N"));
    mToggleInsightLensTrackingAction->setCheckable(true);
    connect(mToggleInsightLensTrackingAction, SIGNAL(triggered(bool)),
            this, SLOT(enableInsightLensTracking(bool)));
    mToggleInsightLensAutoZoomAction = new QAction("Toggle Insight Lens Auto Zoom", this);
    addAction(mToggleInsightLensAutoZoomAction);
    mToggleInsightLensAutoZoomAction->setShortcut(QKeySequence("Shift+Ctrl+M"));
    mToggleInsightLensAutoZoomAction->setCheckable(true);
    connect(mToggleInsightLensAutoZoomAction, SIGNAL(triggered(bool)),
            this, SLOT(enableInsightLensAutoZoom(bool)));
    mToggleInsightLensSingleLayerModeAction = new QAction("Toggle Insight Lens Single Layer Mode", this);
    addAction(mToggleInsightLensSingleLayerModeAction);
    mToggleInsightLensSingleLayerModeAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    mToggleInsightLensSingleLayerModeAction->setCheckable(true);
    connect(mToggleInsightLensSingleLayerModeAction, SIGNAL(triggered(bool)),
            this, SLOT(enableInsightLensSingleLayerMode(bool)));
    mShowBoardInsightPopUpMenuAction = new QAction("Show Board Insight Pop-Up Menu", this);
    addAction(mShowBoardInsightPopUpMenuAction);
    mShowBoardInsightPopUpMenuAction->setShortcut(QKeySequence(Qt::Key_F2));
    connect(mShowBoardInsightPopUpMenuAction, SIGNAL(triggered()),
            this, SLOT(showBoardInsightPopUpMenu()));

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

void PcbEditorWidget::createMenus()
{
    mBoardInsightPopUpMenu = new QMenu("Board Insight Pop-Up Menu", this);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpDisplayAction);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpTrackingAction);
    mBoardInsightPopUpMenu->addAction(mResetHeadsUpDeltaOriginAction);
    mBoardInsightPopUpMenu->addAction(mToggleHeadsUpDeltaOriginAction);
    mBoardInsightPopUpMenu->addSeparator();
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensAction);
    mBoardInsightPopUpMenu->addAction(mShiftInsightLensToMouseAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensTrackingAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensAutoZoomAction);
    mBoardInsightPopUpMenu->addAction(mToggleInsightLensSingleLayerModeAction);
}

void PcbEditorWidget::enableHeadsUp(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableHeadsUpTracking(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::resetHeadsUpDeltaOrigin()
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableHeadsUpDeltaOrigin(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableInsightLens(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::shiftInsightLensToMouse()
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableInsightLensTracking(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableInsightLensAutoZoom(bool enabled)
{
    qDebug() << __FUNCTION__;

}

void PcbEditorWidget::enableInsightLensSingleLayerMode(bool enabled)
{
    qDebug() << __FUNCTION__;
}

void PcbEditorWidget::showBoardInsightPopUpMenu()
{
    mBoardInsightPopUpMenu->exec(QCursor::pos());
}

void PcbEditorWidget::showColorDialog()
{
    qDebug() << __FUNCTION__;
    PcbPaletteManagerDialog dlg;
    dlg.initialise();
    dlg.exec();
}
