#ifndef GRIDTABLEMODEL_H
#define GRIDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

class IGrid;

class GridTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GridTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    IGrid *grid(const QModelIndex &index);

signals:

public slots:
    void reload();

private:
    QList<IGrid *> mGrids;
};

#endif // GRIDTABLEMODEL_H
