#include "mainwindow.h"
#include "logcat1.h"
#include "loggingmodel.h"
#include "logmessage.h"

#include <QApplication>

MainWindow *window;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    window->loggingModel()->addMessage(new LogMessage(type, msg, context.category));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logcat1 *logc1 = new logcat1(&a);
    qInstallMessageHandler(myMessageOutput);
    window = new MainWindow();
    window->show();

    return a.exec();
}
