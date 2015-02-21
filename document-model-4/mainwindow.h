#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPersistentModelIndex>

class QtProperty;
class QUndoGroup;
class QUndoView;

class Document;
class DocumentObject;
class DocumentModelExplorerWidget;
class DocumentObjectPropertyBrowserWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onDocumentModificationChanged(bool isModified);
    void onDocumentUrlChanged(const QUrl &url);
    void onObjectSelectionChanged(const QPersistentModelIndex &index);
    void onPropertyValueChanged(const QPersistentModelIndex &index, QtProperty *property,
                                const QVariant &value, const QVariant &previousValue);
    void updateWindowTitle();

protected slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    void exitApplication();
    void undo();
    void redo();

    void closeEvent(QCloseEvent *event);

protected:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void loadSettings();
    void saveSettings();
    bool maybeSaveFile();
    void initModel();

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_closeAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_exitAction;
    QAction *m_undoAction;
    QAction *m_redoAction;

    QUndoGroup *m_undoGroup;
    QUndoView *m_undoView;

    Document *m_document;
    DocumentModelExplorerWidget *m_explorer;
    DocumentObjectPropertyBrowserWidget *m_browser;
};

#endif // MAINWINDOW_H
