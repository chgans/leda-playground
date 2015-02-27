#include "logcat2.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(log2, "log.2", QtDebugMsg)

logcat2::logcat2(QObject *parent) : QObject(parent)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(doIt()));
    m_timer.start(0);
}

logcat2::~logcat2()
{

}

void logcat2::doIt()
{
    switch (qrand() % 3) {
    case 0:
        qCDebug(log2) << QString("x = %1").arg(qrand());
        break;
    case 1:
        qCWarning(log2) << QString("Foo %1").arg(qrand());
        break;
    case 2:
        qCCritical(log2) << QString("Bar%1Baz").arg(qrand());
        break;
    }
    m_timer.start(qrand() % 5000);
}

