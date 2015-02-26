#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <QtGlobal>
#include <QString>

class LogMessage
{
public:
    LogMessage();
    LogMessage(QtMsgType messageType, const QString &text, const char *categoryName = 0);
    ~LogMessage();

    QtMsgType messageType;
    QString text;
    const char *categoryName;

};
/*
bool operator==(const LogMessage &t1, const LogMessage &t2);
uint qHash(const LogMessage &LogMessage);
bool operator<(const LogMessage &a, const LogMessage &b);
*/
#endif // LOGMESSAGE_H
