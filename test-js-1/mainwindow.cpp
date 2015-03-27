#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mJSEngine = new QJSEngine(this);
    QJSValue val;
    val = mJSEngine->newQObject(this);
    mJSEngine->globalObject().setProperty("gui", val);
    val = mJSEngine->newQObject(ui->textEdit);
    mJSEngine->globalObject().setProperty("text", val);
    val = mJSEngine->newQObject(new JSMath(this));
    mJSEngine->globalObject().setProperty("math", val);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(execJS()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::execJS()
{
    QString cmd = ui->lineEdit->text();
    QJSValue res = mJSEngine->evaluate(cmd);
    if (res.isError())
        qDebug() << "Exception caught:" << res.toString();
    else if (!res.isNull() && !res.isUndefined()) {
        qDebug() << res.toString();
    }
}

