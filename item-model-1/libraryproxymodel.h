#ifndef LIBRARYPROXYMODEL_H
#define LIBRARYPROXYMODEL_H

#include <QAbstractProxyModel>

class LibraryProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    LibraryProxyModel(QObject *parent);
    ~LibraryProxyModel();


    /*
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    */
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

private:

};

#endif // LIBRARYPROXYMODEL_H
