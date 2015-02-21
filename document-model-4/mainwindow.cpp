#include "mainwindow.h"
#include "document.h"
#include "documentmodel.h"
#include "documentobject.h"
#include "documentcommand.h"
#include "documentstupidobject.h"
#include "documentmodelexplorerwidget.h"
#include "documentobjectpropertybrowserwidget.h"

#include "3rdParty/qtpropertybrowser/qtpropertybrowser.h" // QtProperty

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QIcon>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QWidget>
#include <QUndoStack>
#include <QUndoGroup>
#include <QUndoView>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    m_undoGroup(new QUndoGroup(this)), m_undoView(new QUndoView),
    m_document(new Document(this)),
    m_explorer(new DocumentModelExplorerWidget(this)),
    m_browser(new DocumentObjectPropertyBrowserWidget(this))
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    connect(m_document, SIGNAL(modificationChanged(bool)),
            this, SLOT(onDocumentModificationChanged(bool)));
    connect(m_document, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onDocumentUrlChanged(QUrl)));

    connect(m_explorer, SIGNAL(selectionChanged(QPersistentModelIndex)),
            this, SLOT(onObjectSelectionChanged(QPersistentModelIndex)));
    connect(m_browser, SIGNAL(valueChanged(QPersistentModelIndex,QtProperty*,QVariant,QVariant)),
            this, SLOT(onPropertyValueChanged(QPersistentModelIndex,QtProperty*,QVariant,QVariant)));

    QWidget *centralWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    centralWidget->setLayout(layout);
    m_explorer->setModel(m_document->model());
    m_browser->setDocumentModel(m_document->model());
    layout->addWidget(m_explorer);
    layout->addWidget(m_browser);
    layout->addWidget(m_undoView);
    setCentralWidget(centralWidget);

    m_undoGroup->addStack(m_document->undoStack());
    m_undoGroup->setActiveStack(m_document->undoStack());
    m_undoView->setGroup(m_undoGroup);
    loadSettings();

    initModel();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onDocumentModificationChanged(bool isModified)
{
    Q_UNUSED(isModified);
    updateWindowTitle();
}

void MainWindow::onDocumentUrlChanged(const QUrl &url)
{
    Q_UNUSED(url);
    updateWindowTitle();
}

void MainWindow::onObjectSelectionChanged(const QPersistentModelIndex &index)
{
    m_browser->setObjectModelIndex(index);
}

void MainWindow::onPropertyValueChanged(const QPersistentModelIndex &index, QtProperty *property,
                                        const QVariant &value, const QVariant &previousValue)
{
    ChangePropertyValueCommand *command = new ChangePropertyValueCommand();
    command->setIndex(index);
    command->setPropertyName(property->propertyName());
    command->setPropertyValue(value, previousValue);
    m_document->executeCommand(command);
}

void MainWindow::updateWindowTitle()
{
    setWindowFilePath(m_document->url().fileName());
    setWindowModified(m_document->isModified());
}

void MainWindow::newFile()
{
    if (!maybeSaveFile())
        return;
    m_document->clear();
}

void MainWindow::openFile()
{
    if (!maybeSaveFile())
        return;

    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_document->setUrl(QUrl::fromLocalFile(fileName));
    if (!m_document->open()) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(m_document->url().toString())
                             .arg(m_document->errorString()));
        QApplication::restoreOverrideCursor();
        return;
    }
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile()
{
    if (m_document->url().isEmpty())
        return saveFileAs();
    return m_document->save();
}

bool MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return m_document->saveAs(fileName);
}

void MainWindow::exitApplication()
{
    close();
}

void MainWindow::undo()
{
    m_undoGroup->undo();
}

void MainWindow::redo()
{
    m_undoGroup->redo();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSaveFile()) {
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::createActions()
{
    m_newAction = new QAction(QIcon::fromTheme("document-new"), "&New", this);
    m_newAction->setShortcuts(QKeySequence::New);
    m_newAction->setStatusTip(tr("Create a new file"));
    connect(m_newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    m_openAction = new QAction(QIcon::fromTheme("document-open"), "&Open", this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open a new file"));
    connect(m_openAction, SIGNAL(triggered()),
            this, SLOT(openFile()));

    m_closeAction = new QAction(QIcon::fromTheme("document-close"), "&Close", this);
    m_closeAction->setShortcuts(QKeySequence::Close);
    m_closeAction->setStatusTip(tr("Close file"));
    connect(m_closeAction, SIGNAL(triggered()),
            this, SLOT(closeFile()));

    m_saveAction = new QAction(QIcon::fromTheme("document-save"), "&Save", this);
    m_saveAction->setShortcuts(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("Save file"));
    connect(m_saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    m_saveAsAction = new QAction(QIcon::fromTheme("document-new"), "Save &as...", this);
    m_saveAsAction->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("Save as a new file"));
    connect(m_saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveFileAs()));

    m_exitAction = new QAction(QIcon::fromTheme("application-exit"), "E&xit", this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Save as a new file"));
    connect(m_exitAction, SIGNAL(triggered()),
            this, SLOT(exitApplication()));

    m_undoAction = new QAction(QIcon::fromTheme("edit-undo"), "&Undo", this);
    m_undoAction->setShortcuts(QKeySequence::Quit);
    m_undoAction->setStatusTip(tr("Undo"));
    connect(m_undoAction, SIGNAL(triggered()),
            this, SLOT(undo()));

    m_redoAction = new QAction(QIcon::fromTheme("edit-redo"), "&Redo", this);
    m_redoAction->setShortcuts(QKeySequence::Quit);
    m_redoAction->setStatusTip(tr("Redo"));
    connect(m_redoAction, SIGNAL(triggered()),
            this, SLOT(redo()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addAction(m_saveAsAction);

    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::loadSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSaveFile()
{
    if (m_document->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::initModel()
{
}
