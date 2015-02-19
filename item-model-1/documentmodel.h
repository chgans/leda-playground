#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <QAbstractItemModel>
#if 0
class DocumentModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    DocumentModel(QObject *parent);
    ~DocumentModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};
#endif
#endif // DOCUMENTMODEL_H
