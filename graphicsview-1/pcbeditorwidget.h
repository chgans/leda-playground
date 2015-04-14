#ifndef PCBEDITORWIDGET_H
#define PCBEDITORWIDGET_H

#include <QWidget>

class QMainWindow;
class QAction;
class QMenu;
class QToolButton;

class MainView;
class Scene;
class LayerBar;
class PcbPalette;
class PcbPaletteManager;
class DesignLayerManager;
class InsightDockWidget;

class PcbEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PcbEditorWidget(QWidget *parent = 0);
    ~PcbEditorWidget();

    MainView *graphicsView() { return mView; }

    void setScene(Scene *scene);

    void activateEditor(QMainWindow *window);

signals:

public slots:

protected slots:
    void showBoardInsightPopUpMenu();

protected:
    void wheelEvent(QWheelEvent *event);

private:
    MainView *mView;
    LayerBar *m_layerBar;
    InsightDockWidget *m_insightDockWidget;
    QToolButton *mSnapButton;
    QToolButton *mMaskButton;
    QToolButton *mClearButton;

    void createActions();

    Scene *scene() const;

    /** 2D Board Insight System **/
private:
    void createBoardInsightMenu();
    QAction *mToggleHeadsUpDisplayAction; // Shift+H
    QAction *mToggleHeadsUpTrackingAction; // Shift+G
    QAction *mResetHeadsUpDeltaOriginAction; // Ins
    QAction *mToggleHeadsUpDeltaOriginAction; // Shit+D
    QAction *mToggleInsightLensAction; // Shift+M
    QAction *mShiftInsightLensToMouseAction; // Shift+Ctrl+N
    QAction *mToggleInsightLensShapeAction; // Shift+B
    QAction *mToggleInsightLensTrackingAction; // Shift+N
    QAction *mToggleInsightLensAutoZoomAction; // Shift+Ctrl+M
    QAction *mToggleInsightLensSingleLayerModeAction; // Shift+Ctrl+S
    QAction *mShowBoardInsightPopUpMenuAction; // F2
    QMenu   *mBoardInsightPopUpMenu;

    QAction *mInsightLensZoomInAction; // Alt+WheelUp
    QAction *mInsightLensZoomOutAction; // Alt+WheelDown

    QAction *mShowInsightSystemViolationAction; // Shift+X
    QAction *mShowInsightSystemOtherAction; // Shift+V
    QAction *mCycleSingleLayerModeAction; // Shift+S

    QAction *m_cycleDisplayLayerMode;
};

#endif // PCBEDITORWIDGET_H
