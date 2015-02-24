#include "mainwindow.h"
#include "logcat1.h"

#include <QApplication>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug %s %s\n", localMsg.constData(), context.category);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning %s %s\n", localMsg.constData(), context.category);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical %s %s\n", localMsg.constData(), context.category);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal %s %s\n", localMsg.constData(), context.category);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    logcat1 *logc1 = new logcat1();

    MainWindow w;
    w.show();

    return a.exec();
}
