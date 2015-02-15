#ifndef DOCUMENTMODELS_H
#define DOCUMENTMODELS_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QAbstractProxyModel>

#include "document.h"
#include "documentitems.h"

class DocumentTreeViewModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DocumentTreeViewModel(QObject *parent = 0);

    void setRootItem(Document *doc, DocumentItem *root);

    int rowCount(const QModelIndex &parent) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    Document *m_doc;
    DocumentItem *m_root;
    DocumentItem *getItem(const QModelIndex &index) const;
};

class DocumentParameterTableViewModel: public QAbstractTableModel
{
public:
        DocumentParameterTableViewModel(QObject *parent = 0) :
            QAbstractTableModel(parent) {}

        void setRootItem(Document *doc, DocumentParameterItemList *root);

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex &index, int role) const;

private:
        Document *m_doc;
        DocumentParameterItemList *m_root;
};

class SymbolCollectionTableViewModel: public QAbstractTableModel
{
public:
        SymbolCollectionTableViewModel(QObject *parent = 0) :
            QAbstractTableModel(parent) {}

        void setRootItem(Document *doc, DocumentSymbolCollectionItem *root) {
            m_doc = doc;
            m_root = root;
        }

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QVariant data(const QModelIndex &index, int role) const;

private:
        Document *m_doc;
        DocumentSymbolCollectionItem *m_root;
};
#endif // DOCUMENTMODELS_H
