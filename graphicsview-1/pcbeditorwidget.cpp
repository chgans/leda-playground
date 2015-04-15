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

    m_view = new MainView();
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_insightDockWidget = new InsightDockWidget();

    m_layerBar = new LayerBar;

    m_snapButton = new QToolButton;
    m_snapButton->setText("Snap");
    m_snapButton->setAutoRaise(true);
    m_snapButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    m_maskButton = new QToolButton;
    m_maskButton->setText("Mask");
    m_maskButton->setAutoRaise(true);
    m_maskButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    m_clearButton = new QToolButton;
    m_clearButton->setText("Clear");
    m_clearButton->setAutoRaise(true);
    m_clearButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    QHBoxLayout *toolLayout = new QHBoxLayout;
    toolLayout->addWidget(m_layerBar);
    toolLayout->addWidget(m_snapButton);
    toolLayout->addWidget(m_maskButton);
    toolLayout->addWidget(m_clearButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_view);
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
    m_view->setScene(scene);
    m_view->scale(0.75, 0.75);
    m_insightDockWidget->attachView(m_view);
}

void PcbEditorWidget::activateEditor(QMainWindow *window)
{
    window->addAction(m_showBoardInsightPopUpMenuAction);
    window->addAction(m_toggleHeadsUpDisplayAction);
    window->addAction(m_toggleHeadsUpTrackingAction);
    window->addAction(m_resetHeadsUpDeltaOriginAction);
    window->addAction(m_toggleHeadsUpDeltaOriginAction);
    window->addAction(m_toggleInsightLensAction);
    window->addAction(m_toggleInsightLensShapeAction);
    window->addAction(m_toggleInsightLensTrackingAction);
    window->addAction(m_shiftInsightLensToMouseAction);
    window->addAction(m_toggleInsightLensAutoZoomAction);
    window->addAction(m_toggleInsightLensSingleLayerModeAction);
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

    m_showBoardInsightPopUpMenuAction = new QAction("Show Board Insight Pop-Up Menu", this);
    m_showBoardInsightPopUpMenuAction->setShortcut(QKeySequence(Qt::Key_F2));
    connect(m_showBoardInsightPopUpMenuAction, &QAction::triggered,
            this, &PcbEditorWidget::showBoardInsightPopUpMenu);

    m_toggleHeadsUpDisplayAction = new QAction("Toggle Heads Up Display", this);
    m_toggleHeadsUpDisplayAction->setShortcut(QKeySequence("Shift+H"));
    m_toggleHeadsUpDisplayAction->setCheckable(true);
    m_toggleHeadsUpDisplayAction->setChecked(m_view->headsUpEnabled());
    connect(m_toggleHeadsUpDisplayAction, &QAction::triggered,
            m_view, &MainView::enableHeadsUp);

    m_toggleHeadsUpTrackingAction = new QAction("Toggle Heads Up Tracking", this);
    m_toggleHeadsUpTrackingAction->setShortcut(QKeySequence("Shift+G"));
    m_toggleHeadsUpTrackingAction->setCheckable(true);
    m_toggleHeadsUpTrackingAction->setChecked(m_view->headsUpTrackingEnabled());
    connect(m_toggleHeadsUpTrackingAction, &QAction::triggered,
            m_view, &MainView::enableHeadsUpTracking);

    m_resetHeadsUpDeltaOriginAction = new QAction("Reset Heads Up Delta Origin", this);
    m_resetHeadsUpDeltaOriginAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(m_resetHeadsUpDeltaOriginAction, &QAction::triggered,
            m_view, &MainView::resetHeadsUpDeltaOrigin);

    m_toggleHeadsUpDeltaOriginAction = new QAction("Toggle Heads Up Delta Origin", this);
    m_toggleHeadsUpDeltaOriginAction->setShortcut(QKeySequence("Shit+D"));
    m_toggleHeadsUpDeltaOriginAction->setCheckable(true);
    m_toggleHeadsUpDeltaOriginAction->setChecked(m_view->headsUpDeltaOriginEnabled());
    connect(m_toggleHeadsUpDeltaOriginAction, &QAction::triggered,
            m_view, &MainView::enableHeadsUpDeltaOrigin);

    m_toggleInsightLensAction = new QAction("Toggle Insight Lens", this);
    m_toggleInsightLensAction->setShortcut(QKeySequence("Shift+M"));
    m_toggleInsightLensAction->setCheckable(true);
    m_toggleInsightLensAction->setChecked(m_view->insightLensEnabled());
    connect(m_toggleInsightLensAction, &QAction::triggered,
            m_view, &MainView::enableInsightLens);

    m_toggleInsightLensShapeAction = new QAction("Toggle Insight Lens shape", this);
    m_toggleInsightLensShapeAction->setShortcut(QKeySequence("Shift+B"));
    connect(m_toggleInsightLensShapeAction, &QAction::triggered,
            m_view, &MainView::toggleInsightLensShape);

    m_shiftInsightLensToMouseAction = new QAction("Shift Insight Lens To Mouse", this);
    m_shiftInsightLensToMouseAction->setShortcut(QKeySequence("Shift+Ctrl+N"));
    connect(m_shiftInsightLensToMouseAction, &QAction::triggered,
            m_view, &MainView::shiftInsightLensToMouse);

    m_toggleInsightLensTrackingAction = new QAction("Toggle Insight Lens Tracking", this);
    m_toggleInsightLensTrackingAction->setShortcut(QKeySequence("Shift+N"));
    m_toggleInsightLensTrackingAction->setCheckable(true);
    m_toggleInsightLensTrackingAction->setChecked(m_view->insightLensMouseTrackingEnabled());
    connect(m_toggleInsightLensTrackingAction, &QAction::triggered,
            m_view, &MainView::enableInsightLensTracking);

    m_toggleInsightLensAutoZoomAction = new QAction("Toggle Insight Lens Auto Zoom", this);
    m_toggleInsightLensAutoZoomAction->setShortcut(QKeySequence("Shift+Ctrl+M"));
    m_toggleInsightLensAutoZoomAction->setCheckable(true);
    m_toggleInsightLensAutoZoomAction->setChecked(m_view->insightLensAutoZoomEnabled());
    connect(m_toggleInsightLensAutoZoomAction, &QAction::triggered,
            m_view, &MainView::enableInsightLensAutoZoom);

    m_toggleInsightLensSingleLayerModeAction = new QAction("Toggle Insight Lens Single Layer Mode", this);
    m_toggleInsightLensSingleLayerModeAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    m_toggleInsightLensSingleLayerModeAction->setCheckable(true);
    m_toggleInsightLensSingleLayerModeAction->setChecked(m_view->insightLensSingleLayerEnabled());
    connect(m_toggleInsightLensSingleLayerModeAction, &QAction::triggered,
            m_view, &MainView::enableInsightLensSingleLayerMode);

    m_cycleDisplayLayerMode = new QAction("Cycle Single Layer Mode", this);
    m_cycleDisplayLayerMode->setShortcut(QKeySequence("Shift+S"));
    connect(m_cycleDisplayLayerMode, &QAction::triggered,
            m_view, &MainView::cycleLayerDisplayMode);
#if 0
    m_insightLensZoomInAction; // Alt+WheelUp
    m_insightLensZoomOutAction; // Alt+WheelDown

    m_showInsightSystemViolationAction; // Shift+X
    m_showInsightSystemOtherAction; // Shift+V
    m_cycleSingleLayerModeAction; // Shift+S
#endif
}

void PcbEditorWidget::createBoardInsightMenu()
{
    m_boardInsightPopUpMenu = new QMenu("Board Insight Pop-Up Menu", this);
    m_boardInsightPopUpMenu->addAction(m_toggleHeadsUpDisplayAction);
    m_boardInsightPopUpMenu->addAction(m_toggleHeadsUpTrackingAction);
    m_boardInsightPopUpMenu->addAction(m_resetHeadsUpDeltaOriginAction);
    m_boardInsightPopUpMenu->addAction(m_toggleHeadsUpDeltaOriginAction);
    m_boardInsightPopUpMenu->addSeparator();
    m_boardInsightPopUpMenu->addAction(m_toggleInsightLensAction);
    m_boardInsightPopUpMenu->addAction(m_toggleInsightLensShapeAction);
    m_boardInsightPopUpMenu->addAction(m_shiftInsightLensToMouseAction);
    m_boardInsightPopUpMenu->addAction(m_toggleInsightLensTrackingAction);
    m_boardInsightPopUpMenu->addAction(m_toggleInsightLensAutoZoomAction);
    m_boardInsightPopUpMenu->addAction(m_toggleInsightLensSingleLayerModeAction);
}


Scene *PcbEditorWidget::scene() const
{
    Q_ASSERT(m_view && m_view->scene());
    return static_cast<Scene *>(m_view->scene());
}

void PcbEditorWidget::showBoardInsightPopUpMenu()
{
    m_boardInsightPopUpMenu->exec(QCursor::pos());
}
