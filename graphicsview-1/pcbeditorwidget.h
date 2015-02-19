#ifndef PCBEDITORWIDGET_H
#define PCBEDITORWIDGET_H

#include <QWidget>

class QAction;
class QMenu;
class QTabBar;
class QToolButton;
class MainView;

class PcbEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PcbEditorWidget(QWidget *parent = 0);

    MainView *graphicsView() { return mView; }





signals:

public slots:

protected:
    void wheelEvent(QWheelEvent *event);

protected slots:
    void activateLayer(int index);

    // TODO: QAction for that
    void activateNextLayer();
    void activatePreviousLayer();
    void activateNextSignalLayer();
    void activatePreviousSignalLayer();

private:
    MainView *mView;
    QTabBar *mTabBar;
    QToolButton *mCurrentLayerButton;
    QToolButton *mLayerSetButton;
    QToolButton *mSnapButton;
    QToolButton *mMaskButton;
    QToolButton *mClearButton;

    void createActions();
    void createMenus();

    /** 2D Board Insight System **/
private:
    QAction *mToggleHeadsUpDisplayAction; // Shift+H
    QAction *mToggleHeadsUpTrackingAction; // Shift+G
    QAction *mResetHeadsUpDeltaOriginAction; // Ins
    QAction *mToggleHeadsUpDeltaOriginAction; // Shit+D
    QAction *mToggleInsightLensAction; // Shift+M
    QAction *mShiftInsightLensToMouseAction; // Shift+Ctrl+N
    QAction *mToggleInsightLensTrackingAction; // Shift+N
    QAction *mToggleInsightLensAutoZoomAction; // Shift+Ctrl+M
    QAction *mToggleInsightLensSingleLayerModeAction; // Shift+Ctrl+S
    QAction *mShowBoardInsightPopUpMenuAction; // F2
    QMenu   *mBoardInsightPopUpMenu;
protected slots:
    void enableHeadsUp(bool enabled);
    void enableHeadsUpTracking(bool enabled);
    void resetHeadsUpDeltaOrigin();
    void enableHeadsUpDeltaOrigin(bool enabled);
    void enableInsightLens(bool enabled);
    void shiftInsightLensToMouse();
    void enableInsightLensTracking(bool enabled);
    void enableInsightLensAutoZoom(bool enabled);
    void enableInsightLensSingleLayerMode(bool enabled);
    void showBoardInsightPopUpMenu();

    void showColorDialog();

private:
    QAction *mInsightLensZoomInAction; // Alt+WheelUp
    QAction *mInsightLensZoomOutAction; // Alt+WheelDown

    QAction *mShowInsightSystemViolationAction; // Shift+X
    QAction *mShowInsightSystemOtherAction; // Shift+V
    QAction *mCycleSingleLayerModeAction; // Shift+S

    QAction *mActivateNextLayerAction; // Ctrl+Shfit+WheelUp or KPD+
    QAction *mActivatePreviousLayerAction; // // Ctrl+Shfit+WheelDown or KPD-
    QAction *mActivateNextSignalLayerAction; // KPD*
    QAction *mActivatePreviousSignalLayerAction; // // KPD/

    QAction *mShowViewConfigurationDialogAction; // L
};

#endif // PCBEDITORWIDGET_H
