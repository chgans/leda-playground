#include "mainwindow.h"
#include "logcat1.h"
#include "logcat2.h"
#include "loggingmodel.h"
#include "logmessage.h"

#include <QApplication>

MainWindow *window;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text = msg;
    if (msg.startsWith('"'))
        text.remove(0, 1);
    if (msg.endsWith('"'))
        text.remove(text.length()-1, 1);

    window->loggingModel()->addMessage(new LogMessage(type, text, context.category));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logcat1 *logc1 = new logcat1(&a);
    logcat2 *logc2 = new logcat2(&a);
    window = new MainWindow();
    qInstallMessageHandler(myMessageOutput);
    window->show();

    return a.exec();
}
