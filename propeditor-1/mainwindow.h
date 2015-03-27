#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class LabelItem;
class TextFrameItem;
class TextFrameItemEditor;
class SvgItem;

class QtProperty;
class QVariant;
class QtTreePropertyBrowser;
class QtVariantPropertyManager;

class QGraphicsScene;
class QGraphicsView;
class QGraphicsObject;

class QLineEdit;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void setProperty(QtProperty *property, const QVariant &value);
    void populateBrowser(QObject *object, const QMetaObject *metaObject);
    void updateBrowser();
    void showHideTextEditorToolBar(bool show);
    void moveTextEditorToolBar();
    void setTextBold();

private:
    Ui::MainWindow *ui;
    QGraphicsView *mView;
    QGraphicsScene *mScene;
    LabelItem *mLabel;
    TextFrameItem *mText;
    TextFrameItemEditor *mTextEditor;
    SvgItem *mSvg;
    QtTreePropertyBrowser *mBrowser;
    QtVariantPropertyManager *mManager;
    QGraphicsObject *mBrowsedObject;
    bool mPopulatingBrowser;
    QLineEdit *mFilterEdit;
    QToolBar *mTextEditToolBar;
};




#endif // MAINWINDOW_H
