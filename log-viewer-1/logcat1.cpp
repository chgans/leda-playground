#include "logcat1.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(log1, "log.1")

logcat1::logcat1(QObject *parent) : QObject(parent)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(doIt()));
    m_timer.start(0);
}

logcat1::~logcat1()
{

}

void logcat1::doIt()
{
    switch (qrand() % 3) {
    case 0:
        qCDebug(log1) << QString("x = %1").arg(qrand());
        break;
    case 1:
        qCWarning(log1) << QString("Foo %1").arg(qrand());
        break;
    case 2:
        qCCritical(log1) << QString("Bar%1Baz").arg(qrand());
        break;
    }
    m_timer.start(qrand() % 5000);
}

