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
    //QtMsgType messageType(const QModelIndex &index) const;

    void addMessage(LogMessage *message);

signals:
    void categoryListChanged(const QList<const char *> &categories);

public slots:
    void clearMessages();

private:
    QList<LogMessage*> m_messages;
    QList<QString> m_messageTypeNames;
    QList<QString> m_messageTypeIconNames;
    QList<const char *> m_categories;
};

#endif // LOGGINGMODEL_H
