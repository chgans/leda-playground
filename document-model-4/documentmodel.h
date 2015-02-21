#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <QAbstractItemModel>

class DocumentObject;

class DocumentModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DocumentModel(QObject *parent = 0);

    void clear();
    void setRootObject(DocumentObject *object);

    DocumentObject *object(const QModelIndex &index) const;
    QModelIndex addProperty(const QModelIndex &index, const QString &name,const QVariant &value);
    void removeProperty(const QModelIndex &index);
    void changeProperty(const QModelIndex &index, const QVariant &value);
    QModelIndex addObject(const QModelIndex &parent, DocumentObject *object);
    void removeObject(const QModelIndex &index);

    // From QAbstractItemModel
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    DocumentObject *m_root;
};

#endif // DOCUMENTMODEL_H
