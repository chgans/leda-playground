#ifndef LOGGINGSORTFILTERPROXYMODEL_H
#define LOGGINGSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "logmessage.h"

class LoggingSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    LoggingSortFilterProxyModel(QObject *parent);
    ~LoggingSortFilterProxyModel();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    bool filterIncludesCategoryName(const char *categoryName) const;
    bool filterIncludesMessageType(QtMsgType messageType) const;
    bool filterIncludesDebugMessages() const;
    bool filterIncludesWarningMessages() const;
    bool filterIncludesCriticalMessages() const;
    bool filterIncludesFatalMessages() const;

public slots:
    void setFilterIncludesMessageType(QtMsgType messageType, bool includes);
    void setFilterIncludesCategoryName(const char *categoryName, bool includes);
    void setFilterIncludesDebugMessages(bool includes);
    void setFilterIncludesWarningMessages(bool includes);
    void setFilterIncludesCriticalMessages(bool includes);
    void setFilterIncludesFatalMessages(bool includes);

private:
    QMap<QtMsgType, bool> m_messageType;
    QMap<const char *, bool> m_category;
};

#endif // LOGGINGSORTFILTERPROXYMODEL_H
