#include "logmessage.h"

LogMessage::LogMessage():
    messageType(QtDebugMsg), categoryName(nullptr)
{

}

LogMessage::LogMessage(QtMsgType messageType, const QString &text, const char *categoryName):
    messageType(messageType), text(text), categoryName(categoryName)
{

}

LogMessage::~LogMessage()
{

}


/*
bool operator==(const LogMessage &t1, const LogMessage &t2)
{
    if (t1.messageType != t2.messageType)
        return false;

}


uint qHash(const LogMessage &LogMessage)
{

}


bool operator<(const LogMessage &a, const LogMessage &b)
{

}
*/
