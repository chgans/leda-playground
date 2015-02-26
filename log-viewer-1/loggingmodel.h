#ifndef LOGGINGMODEL_H
#define LOGGINGMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "logmessage.h"

class LoggingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LoggingModel(QObject *parent = 0);
    ~LoggingModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    LogMessage *message(const QModelIndex &index) const;

    void addMessage(LogMessage *message);
    void clearMessages();

private:
    QList<LogMessage*> m_messages;
    QList<QString> m_messageTypeNames;
};

#endif // LOGGINGMODEL_H
