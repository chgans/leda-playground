
#ifndef UngroupProxyModel_H
#define UngroupProxyModel_H

#include <QAbstractProxyModel>

class UngroupProxyModel : public QAbstractProxyModel {
Q_OBJECT
public:
    UngroupProxyModel(QObject *);
    virtual ~UngroupProxyModel();

    virtual void setSourceModel(QAbstractItemModel *);

    virtual QModelIndex mapFromSource(const QModelIndex &) const;
    virtual QModelIndex mapToSource(const QModelIndex &) const;

    virtual QModelIndex	parent(const QModelIndex &) const;
    virtual QModelIndex index(int, int, const QModelIndex & p = QModelIndex()) const;
    
    virtual int	rowCount(const QModelIndex & p = QModelIndex()) const;
    virtual int	columnCount(const QModelIndex & p = QModelIndex()) const;

private slots:
    void source_rowsAboutToBeInserted(QModelIndex, int, int);
    void source_rowsAboutToBeRemoved(QModelIndex, int, int);
    void source_rowsInserted(QModelIndex, int, int);
    void source_rowsRemoved(QModelIndex, int, int);
    void source_dataChanged(QModelIndex, QModelIndex);
    void source_modelReset();

private:
    class Private;
    Private * d;
};

#endif // UngroupProxyModel_H
