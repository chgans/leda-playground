#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H


#include <QAbstractItemModel>
#include <QIcon>
#include <QString>

/*
 * + Component
 *   + sym1
 *   + sym2
 *   + pack1
 *     + fp1
 *     + bd1
 *   + pack2
 *     + fp1
 *     + bd1
 *
 */
class TreeItem
{
public:
    explicit TreeItem(const QIcon &icon, const QString &name, TreeItem *parent = 0):
        mIcon(icon), mName(name), mParentItem(parent)
    {
    }

    ~TreeItem()
    {
        qDeleteAll(mChildItems);
    }

    void appendChild(TreeItem *child)
    {
        mChildItems.append(child);
    }

    TreeItem *child(int row)
    {
        return mChildItems.value(row);
    }

    int childCount() const
    {
        return mChildItems.count();
    }

    int columnCount() const
    {
        return 2;
    }

    QVariant data(int column) const
    {
        if (column == 0)
            return mIcon;
        else if (column == 1)
            return mName;
        else
            return QVariant();
    }

    int row() const
    {
        if (mParentItem)
            return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));
        return 0;
    }

    TreeItem *parent()
    {
        return mParentItem;
    }

private:
    QIcon mIcon;
    QString mName;
    TreeItem *mParentItem;
    QList<TreeItem*> mChildItems;
};


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(const QString &data, QObject *parent = 0)
    {
        QList<QVariant> rootData;
        rootData << "Title" << "Summary";
        mRootItem = new TreeItem(QIcon(), "Name");
        //setupModelData();
    }

    ~TreeModel()
    {
        delete mRootItem;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

        if (role == Qt::DecorationRole)
            return item->data(0);
        else if (role == Qt::DisplayRole)
            return item->data(1);
        else
            return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return 0;

        return QAbstractItemModel::flags(index);
    }

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return mRootItem->data(section);

        return QVariant();
    }

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        TreeItem *parentItem;

        if (!parent.isValid())
            parentItem = mRootItem;
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());

        TreeItem *childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &index) const
    {
        if (!index.isValid())
            return QModelIndex();

        TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
        TreeItem *parentItem = childItem->parent();

        if (parentItem == mRootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        TreeItem *parentItem;
        if (parent.column() > 0)
            return 0;

        if (!parent.isValid())
            parentItem = mRootItem;
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());

        return parentItem->childCount();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return 1;
        if (parent.isValid())
            return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
        else
            return mRootItem->columnCount();
    }

    /*
    * + Component
    *   + sym1
    *   + sym2
    *   + pack1
    *     + fp1
    *     + bd1
    *   + pack2
    *     + fp1
    *     + bd1
    */
    void setupModelData(/*const QStringList &lines, TreeItem *parent*/)
    {
        TreeItem *root = mRootItem;
        TreeItem *troot;
        TreeItem *item;
        item = new TreeItem(QIcon(":/icons/component.png"), "Resistor", root);
        root->appendChild(item);

        root = item;
        item = new TreeItem(QIcon(":/icons/symbol.png"), "Res1", root);
        root->appendChild(item);
        item = new TreeItem(QIcon(":/icons/symbol.png"), "Res2", root);
        root->appendChild(item);

        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "0402", r);
            r->appendChild(item);
            r = item;
            item = new TreeItem(QIcon(":/icons/footprint.png"), "0402", r);
            r->appendChild(item);
            item = new TreeItem(QIcon(":/icons/body.png"), "0402", r);
            r->appendChild(item);
        }
        item = new TreeItem(QIcon(":/icons/sigint.png"), "Res", root);
        root->appendChild(item);
        item = new TreeItem(QIcon(":/icons/simu.png"), "Res", root);
        root->appendChild(item);

        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "0603_M", r);
            r->appendChild(item);
            r = item;
            item = new TreeItem(QIcon(":/icons/footprint.png"), "0603", r);
            r->appendChild(item);
            item = new TreeItem(QIcon(":/icons/body.png"), "0603", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "1005_M", r);
            r->appendChild(item);
            r = item;
            item = new TreeItem(QIcon(":/icons/footprint.png"), "1005", r);
            r->appendChild(item);
            item = new TreeItem(QIcon(":/icons/body.png"), "1005", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "1608_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "2012_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "3216_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "3225_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "4532_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "5025_M", r);
            r->appendChild(item);
        }
        {
            TreeItem *r = root;
            item = new TreeItem(QIcon(":/icons/package.png"), "6432_M", r);
            r->appendChild(item);
        }
    }
private:
    TreeItem *mRootItem;
};




#include <QDockWidget>

namespace Ui {
class DockWidget;
}

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWidget(QWidget *parent = 0);
    ~DockWidget();

private:
    Ui::DockWidget *ui;
};

#endif // DOCKWIDGET_H
