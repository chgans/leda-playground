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
#include "insightdockwidget.h"

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

    mView = new MainView();
    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_insightDockWidget = new InsightDockWidget();

    m_layerBar = new LayerBar;

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
    toolLayout->addWidget(m_layerBar);
    toolLayout->addWidget(mSnapButton);
    toolLayout->addWidget(mMaskButton);
    toolLayout->addWidget(mClearButton);

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
    m_insightDockWidget->attachView(mView);
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

    window->addDockWidget(Qt::LeftDockWidgetArea, m_insightDockWidget);
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
    connect(mShowBoardInsightPopUpMenuAction, &QAction::triggered,
            this, &PcbEditorWidget::showBoardInsightPopUpMenu);

    mToggleHeadsUpDisplayAction = new QAction("Toggle Heads Up Display", this);
    mToggleHeadsUpDisplayAction->setShortcut(QKeySequence("Shift+H"));
    mToggleHeadsUpDisplayAction->setCheckable(true);
    mToggleHeadsUpDisplayAction->setChecked(mView->headsUpEnabled());
    connect(mToggleHeadsUpDisplayAction, &QAction::triggered,
            mView, &MainView::enableHeadsUp);

    mToggleHeadsUpTrackingAction = new QAction("Toggle Heads Up Tracking", this);
    mToggleHeadsUpTrackingAction->setShortcut(QKeySequence("Shift+G"));
    mToggleHeadsUpTrackingAction->setCheckable(true);
    mToggleHeadsUpTrackingAction->setChecked(mView->headsUpTrackingEnabled());
    connect(mToggleHeadsUpTrackingAction, &QAction::triggered,
            mView, &MainView::enableHeadsUpTracking);

    mResetHeadsUpDeltaOriginAction = new QAction("Reset Heads Up Delta Origin", this);
    mResetHeadsUpDeltaOriginAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(mResetHeadsUpDeltaOriginAction, &QAction::triggered,
            mView, &MainView::resetHeadsUpDeltaOrigin);

    mToggleHeadsUpDeltaOriginAction = new QAction("Toggle Heads Up Delta Origin", this);
    mToggleHeadsUpDeltaOriginAction->setShortcut(QKeySequence("Shit+D"));
    mToggleHeadsUpDeltaOriginAction->setCheckable(true);
    mToggleHeadsUpDeltaOriginAction->setChecked(mView->headsUpDeltaOriginEnabled());
    connect(mToggleHeadsUpDeltaOriginAction, &QAction::triggered,
            mView, &MainView::enableHeadsUpDeltaOrigin);

    mToggleInsightLensAction = new QAction("Toggle Insight Lens", this);
    mToggleInsightLensAction->setShortcut(QKeySequence("Shift+M"));
    mToggleInsightLensAction->setCheckable(true);
    mToggleInsightLensAction->setChecked(mView->insightLensEnabled());
    connect(mToggleInsightLensAction, &QAction::triggered,
            mView, &MainView::enableInsightLens);

    mToggleInsightLensShapeAction = new QAction("Toggle Insight Lens shape", this);
    mToggleInsightLensShapeAction->setShortcut(QKeySequence("Shift+B"));
    connect(mToggleInsightLensShapeAction, &QAction::triggered,
            mView, &MainView::toggleInsightLensShape);

    mShiftInsightLensToMouseAction = new QAction("Shift Insight Lens To Mouse", this);
    mShiftInsightLensToMouseAction->setShortcut(QKeySequence("Shift+Ctrl+N"));
    connect(mShiftInsightLensToMouseAction, &QAction::triggered,
            mView, &MainView::shiftInsightLensToMouse);

    mToggleInsightLensTrackingAction = new QAction("Toggle Insight Lens Tracking", this);
    mToggleInsightLensTrackingAction->setShortcut(QKeySequence("Shift+N"));
    mToggleInsightLensTrackingAction->setCheckable(true);
    mToggleInsightLensTrackingAction->setChecked(mView->insightLensMouseTrackingEnabled());
    connect(mToggleInsightLensTrackingAction, &QAction::triggered,
            mView, &MainView::enableInsightLensTracking);

    mToggleInsightLensAutoZoomAction = new QAction("Toggle Insight Lens Auto Zoom", this);
    mToggleInsightLensAutoZoomAction->setShortcut(QKeySequence("Shift+Ctrl+M"));
    mToggleInsightLensAutoZoomAction->setCheckable(true);
    mToggleInsightLensAutoZoomAction->setChecked(mView->insightLensAutoZoomEnabled());
    connect(mToggleInsightLensAutoZoomAction, &QAction::triggered,
            mView, &MainView::enableInsightLensAutoZoom);

    mToggleInsightLensSingleLayerModeAction = new QAction("Toggle Insight Lens Single Layer Mode", this);
    mToggleInsightLensSingleLayerModeAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    mToggleInsightLensSingleLayerModeAction->setCheckable(true);
    mToggleInsightLensSingleLayerModeAction->setChecked(mView->insightLensSingleLayerEnabled());
    connect(mToggleInsightLensSingleLayerModeAction, &QAction::triggered,
            mView, &MainView::enableInsightLensSingleLayerMode);

    m_cycleDisplayLayerMode = new QAction("Cycle Single Layer Mode", this);
    m_cycleDisplayLayerMode->setShortcut(QKeySequence("Shift+S"));
    connect(m_cycleDisplayLayerMode, &QAction::triggered,
            mView, &MainView::cycleLayerDisplayMode);
#if 0
    mInsightLensZoomInAction; // Alt+WheelUp
    mInsightLensZoomOutAction; // Alt+WheelDown

    mShowInsightSystemViolationAction; // Shift+X
    mShowInsightSystemOtherAction; // Shift+V
    mCycleSingleLayerModeAction; // Shift+S
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


Scene *PcbEditorWidget::scene() const
{
    Q_ASSERT(mView && mView->scene());
    return static_cast<Scene *>(mView->scene());
}

void PcbEditorWidget::showBoardInsightPopUpMenu()
{
    mBoardInsightPopUpMenu->exec(QCursor::pos());
}
