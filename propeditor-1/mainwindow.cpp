#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "3rdParty/misc/richtexteditor_p.h"
#include "3rdParty/qtgradienteditor/qtgradienteditor.h"
#include "3rdParty/qtpropertybrowser/qttreepropertybrowser.h"
#include "3rdParty/qtpropertybrowser/qtvariantproperty.h"
using namespace qdesigner_internal;

#include "labelitem.h"
#include "svgitem.h"
#include "textframeitem.h"
#include "textframeitemeditor.h"

#include <QDockWidget>
#include <QMetaProperty>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QTextEdit>

#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolBar>
#include <QComboBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mScene = new QGraphicsScene(0, 0, 500, 500, this);
    mView = new QGraphicsView;
    mView->setScene(mScene);
    mView->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    setCentralWidget(mView);

    /*
    mLabel = new LabelItem();
    mLabel->setProperty("text", "Hello world!");
    mLabel->setPos(250, 250);
    mLabel->setFlag(LabelItem::ItemIsSelectable);
    mLabel->setFlag(LabelItem::ItemIsMovable);
    mScene->addItem(mLabel);

    mSvg = new SvgItem();
    mSvg->setProperty("fileName", qApp->applicationDirPath().append("/symbol.svg"));
    mSvg->setPos(100, 100);
    mSvg->setFlag(SvgItem::ItemIsSelectable);
    mSvg->setFlag(SvgItem::ItemIsMovable);
    mScene->addItem(mSvg);
    */

    mText = new TextFrameItem();
    mText->setPlainText("Hello world!\nHello world!\nHello world!");
    mText->setPos(250, 300);
    mText->setFlag(LabelItem::ItemIsSelectable);
    mText->setFlag(LabelItem::ItemIsMovable);
    mText->setTextWidth(100);
    mScene->addItem(mText);
    mTextEditor = new TextFrameItemEditor(this);

    connect(mText, SIGNAL(focusChanged(bool)),
            this, SLOT(showHideTextEditorToolBar(bool)));

#if 0
    mTextEditToolBar = new QToolBar("Text tools", mView);
    //addToolBar(mTextEditToolBar);
    mTextEditToolBar->setVisible(false);
    mTextEditToolBar->setFloatable(true);
    mTextEditToolBar->setMovable(true);
    mTextEditToolBar->setAutoFillBackground(true);
    QAction *action;
    action = new QAction(QIcon(":/images/textbold.png"), "Bold", this);
    mTextEditToolBar->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT());
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textitalic.png"), "Italic", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textunder.png"), "Underline", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textsubscript.png"), "Subscript", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textsuperscript.png"), "Superscript", this));
    mTextEditToolBar->addSeparator();
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textleft.png"), "Align left", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textcenter.png"), "Align center", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textright.png"), "Align right", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textjustify.png"), "Justify", this));
    mTextEditToolBar->addSeparator();
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/textanchor.png"), "Insert hyperlink", this));
    mTextEditToolBar->addAction(new QAction(QIcon(":/images/insertimage.png"), "Insert image", this));
    mTextEditToolBar->addSeparator();
    mTextEditToolBar->addAction(new QAction("Editor...", this));
#endif


#if 0
    QTextEdit *edit = new QTextEdit;
    edit->setReadOnly(true);
    edit->setText("<b>Hello</b> <em>world</em>!");
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    proxy->setWidget(edit);
    proxy->setFlag(LabelItem::ItemIsSelectable);
    proxy->setFlag(LabelItem::ItemIsMovable);
    proxy->setSelected(true);
    mScene->addItem(proxy);
#endif

    mBrowsedObject = 0;
    mBrowser = new QtTreePropertyBrowser;
    mBrowser->setPropertiesWithoutValueMarked(true);
    mManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *mFactory = new QtVariantEditorFactory(this);
    mBrowser->setFactoryForManager(mManager, mFactory);

    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout;
    mFilterEdit = new QLineEdit;
    l->addWidget(mFilterEdit);
    l->addWidget(mBrowser);
    w->setLayout(l);

    QDockWidget *mDock = new QDockWidget("Properties");
    mDock->setWidget(w);
    addDockWidget(Qt::RightDockWidgetArea, mDock);

    connect(mManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
            this, SLOT(setProperty(QtProperty*,QVariant)));

    connect(mScene, SIGNAL(selectionChanged()),
            this, SLOT(updateBrowser()));
    updateBrowser();

    //RichTextEditorDialog *dlg = new RichTextEditorDialog;
    //dlg->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProperty(QtProperty *property, const QVariant &value)
{
    if (mPopulatingBrowser)
        return;

    //qDebug() << Q_FUNC_INFO << property->propertyName() << value;
    QByteArray name = property->propertyName().toLatin1();
    bool ok = mBrowsedObject->setProperty(name.constData(), value);
    if (!ok) {
        qDebug() << QString("Failed to set property %1:%2 (%3)").arg(mBrowsedObject->metaObject()->className()).arg(property->propertyName()).arg(value.typeName());
    }
    //qDebug() << Q_FUNC_INFO << mBrowsedObject->property(name.constData());
}

void MainWindow::populateBrowser(QObject *object, const QMetaObject *metaObject)
{
    if (!object || !metaObject)
        return;

    populateBrowser(object, metaObject->superClass());
    QtProperty *parentProp = mManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                   metaObject->className());
    for (int i=0; i<metaObject->propertyCount(); ++i) {
        QMetaProperty metaProp = metaObject->property(i);
        if (metaProp.enclosingMetaObject() != metaObject) {
            //qDebug() << metaObject->className() << metaProp.enclosingMetaObject()->className() << metaProp.name();
            continue;
        }
        //qDebug() << metaObject->className() << metaProp.name();
        QtProperty *prop = mManager->addProperty(metaProp.type(),
                                                 metaProp.name());
        mManager->setValue(prop, object->property(metaProp.name()));
        if (!prop) {
            qDebug() << QString("Cannot add property %1:%2 of type %3").arg(metaObject->className()).arg(metaProp.name()).arg(metaProp.typeName());
            continue;
        }
        prop->setEnabled(metaObject->property(i).isWritable());
        parentProp->addSubProperty(prop);
    }
    QtBrowserItem *item = mBrowser->addProperty(parentProp);
    foreach (QtBrowserItem *child, item->children()) {
        mBrowser->setExpanded(child, false);
    }
}

void MainWindow::updateBrowser()
{
    mManager->clear();
    mBrowser->clear();
    mBrowsedObject = 0;
    if (mScene->selectedItems().isEmpty())
        return;

    mBrowsedObject = dynamic_cast<QGraphicsObject*>(mScene->selectedItems()[0]);
    if (!mBrowsedObject)
        return;

    mPopulatingBrowser = true;
    populateBrowser(mBrowsedObject, mBrowsedObject->metaObject());
    mPopulatingBrowser = false;
}

void MainWindow::showHideTextEditorToolBar(bool show)
{
#if 0
    mTextEditToolBar->setVisible(show);
    if (show) {
        connect(mBrowsedObject, SIGNAL(xChanged()),
                this, SLOT(moveTextEditorToolBar()));
        connect(mBrowsedObject, SIGNAL(yChanged()),
                this, SLOT(moveTextEditorToolBar()));
        moveTextEditorToolBar();
    }
    else {
        disconnect(mBrowsedObject, SIGNAL(xChanged()),
                   this, SLOT(moveTextEditorToolBar()));
        disconnect(mBrowsedObject, SIGNAL(yChanged()),
                   this, SLOT(moveTextEditorToolBar()));
    }
#endif
    if (show)
        mTextEditor->attachItem(mText, mView);
    else
        mTextEditor->detachItem();
}

void MainWindow::moveTextEditorToolBar()
{
    // Move toolbar above item, and center-align it horizontally
    QPoint pos = mView->mapFromScene(mBrowsedObject->scenePos());
    int itemWidth = mView->mapFromScene(mBrowsedObject->boundingRect().width(), 0).x();
    int barWidth = mTextEditToolBar->width();
    pos.ry() -= mTextEditToolBar->height();
    pos.rx() -= (barWidth - itemWidth )/ 2.0;
    mTextEditToolBar->move(pos);
}

void MainWindow::setTextBold()
{
    mText->document();
}
