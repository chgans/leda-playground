#include "loggingmodel.h"
#include <QIcon>

LoggingModel::LoggingModel(QObject *parent):
    QAbstractTableModel(parent)
{
    m_messageTypeNames.append(QString("Debug"));
    m_messageTypeNames.append(QString("Warning"));
    m_messageTypeNames.append(QString("Critical"));
    m_messageTypeNames.append(QString("Fatal"));

    m_messageTypeIconNames.append(QString("dialog-information"));
    m_messageTypeIconNames.append(QString("dialog-warning"));
    m_messageTypeIconNames.append(QString("dialog-error"));
    m_messageTypeIconNames.append(QString("dialog-error"));
}

LoggingModel::~LoggingModel()
{

}

int LoggingModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_messages.count();
}

int LoggingModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}


QVariant LoggingModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_ASSERT(index.row() < m_messages.count());
    Q_ASSERT(index.column() < 2);

    LogMessage *msg = message(index);

    switch (role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            return QVariant(QString(QByteArray(msg->categoryName)));
        case 1:
            return QVariant(msg->text);
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        switch(index.column())
        {
        case 0:
            return QVariant(QIcon::fromTheme(m_messageTypeIconNames[msg->messageType]));
        case 1:
            return QVariant();
        default:
            return QVariant();
        }
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
    int pos = m_messages.count();

    beginInsertRows(QModelIndex(), pos, pos);
    m_messages.append(message);
    endInsertRows();
    if (!m_categories.contains(message->categoryName)) {
        m_categories.append(message->categoryName);
        emit categoryListChanged(m_categories);
    }
}

void LoggingModel::clearMessages()
{
    beginResetModel();
    qDeleteAll(m_messages);
    m_messages.clear();
    endResetModel();
    if (m_categories.count() > 0) {
        m_categories.clear();
        emit categoryListChanged(m_categories);
    }
}

