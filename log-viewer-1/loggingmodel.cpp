#include "loggingmodel.h"

LoggingModel::LoggingModel(QObject *parent):
    QAbstractTableModel(parent)
{
    m_messageTypeNames.append(QString("Debug"));
    m_messageTypeNames.append(QString("Warning"));
    m_messageTypeNames.append(QString("Critical"));
    m_messageTypeNames.append(QString("Fatal"));
}

LoggingModel::~LoggingModel()
{

}

int LoggingModel::rowCount(const QModelIndex &parent) const
{
    return m_messages.count();
}

int LoggingModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant LoggingModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Q_ASSERT(index.row() < m_messages.count());
    Q_ASSERT(index.column() < 3);
    LogMessage *msg = message(index);
    switch(index.column())
    {
    case 0:
        return QVariant(m_messageTypeNames[msg->messageType]);
    case 1:
        return QVariant(QString(QByteArray(msg->categoryName)));
    case 2:
        return QVariant(msg->text);
    default:
        return QVariant();
    }
}

LogMessage *LoggingModel::message(const QModelIndex &index) const
{
    int row = index.row();
    if (!index.isValid() || row < 0 || row >= m_messages.count())
        return 0;
    return m_messages.value(row);
}

void LoggingModel::addMessage(LogMessage *message)
{
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    m_messages.append(message);
    endInsertRows();
}

void LoggingModel::clearMessages()
{
    beginResetModel();
    qDeleteAll(m_messages);
    m_messages.clear();
    endResetModel();
}

