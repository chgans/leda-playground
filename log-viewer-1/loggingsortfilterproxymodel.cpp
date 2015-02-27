#include "loggingsortfilterproxymodel.h"
#include "loggingmodel.h"

LoggingSortFilterProxyModel::LoggingSortFilterProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    m_messageType[QtDebugMsg] = true;
    m_messageType[QtWarningMsg] = true;
    m_messageType[QtCriticalMsg] = true;
    m_messageType[QtFatalMsg] = true;
}

LoggingSortFilterProxyModel::~LoggingSortFilterProxyModel()
{

}

bool LoggingSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    LoggingModel *model = static_cast<LoggingModel *>(sourceModel());
    QModelIndex index = model->index(source_row, 0, source_parent);
    LogMessage *msg = model->message(index);
    QtMsgType type = msg->messageType;
    Q_ASSERT(type >= 0 && type < 4);
    const char *category = msg->categoryName;
    return filterIncludesMessageType(type) &&
            filterIncludesCategoryName(category) &&
            QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool LoggingSortFilterProxyModel::filterIncludesCategoryName(const char *categoryName) const
{
    if (!m_category.contains(categoryName))
        return true;
    return m_category.value(categoryName);
}

bool LoggingSortFilterProxyModel::filterIncludesMessageType(QtMsgType messageType) const
{
    return m_messageType.value(messageType, false);
}

void LoggingSortFilterProxyModel::setFilterIncludesMessageType(QtMsgType messageType, bool includes)
{
    Q_ASSERT(messageType >= 0 && messageType < 4);
    m_messageType[messageType] = includes;
    invalidateFilter();
}

void LoggingSortFilterProxyModel::setFilterIncludesCategoryName(const char *categoryName, bool includes)
{
    m_category[categoryName] = includes;
    invalidateFilter();
}

bool LoggingSortFilterProxyModel::filterIncludesDebugMessages() const
{
    return filterIncludesMessageType(QtDebugMsg);
}

bool LoggingSortFilterProxyModel::filterIncludesWarningMessages() const
{
    return filterIncludesMessageType(QtWarningMsg);
}

bool LoggingSortFilterProxyModel::filterIncludesCriticalMessages() const
{
    return filterIncludesMessageType(QtCriticalMsg);
}

bool LoggingSortFilterProxyModel::filterIncludesFatalMessages() const
{
    return filterIncludesMessageType(QtFatalMsg);
}

void LoggingSortFilterProxyModel::setFilterIncludesDebugMessages(bool includes)
{
    setFilterIncludesMessageType(QtDebugMsg, includes);
}

void LoggingSortFilterProxyModel::setFilterIncludesWarningMessages(bool includes)
{
    setFilterIncludesMessageType(QtWarningMsg, includes);
}

void LoggingSortFilterProxyModel::setFilterIncludesCriticalMessages(bool includes)
{
    setFilterIncludesMessageType(QtCriticalMsg, includes);
}

void LoggingSortFilterProxyModel::setFilterIncludesFatalMessages(bool includes)
{
    setFilterIncludesMessageType(QtFatalMsg, includes);
}

