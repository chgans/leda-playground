#ifndef DOCUMENTITEM_H
#define DOCUMENTITEM_H

#include <QString>
#include <QVariant>
#include <QStandardItem>

// TBD: add clone() and operator ==() here
class IDocumentItem: public QStandardItem
{
public:
    explicit IDocumentItem():
        QStandardItem()
    {}

    virtual ~IDocumentItem()
    {}

    enum ItemType {
        RootItemType = 0,
        LibraryItemType,
        PackageItemType,
    };
};

class GenericItem: public IDocumentItem
{
public:
    GenericItem():
        IDocumentItem()
    {
        setColumnCount(1);
        setRowCount(4);
        auto nameItem = new QStandardItem();
        nameItem->setData("", Qt::DisplayRole);
        setChild(0, 0, nameItem);
        auto valItem = new QStandardItem();
        valItem->setData("", Qt::DisplayRole);
        setChild(1, 0, valItem);
        auto typeItem = new QStandardItem();
        typeItem->setData(QVariant::String, Qt::DisplayRole);
        setChild(2, 0, typeItem);
        auto roItem = new QStandardItem();
        roItem->setData(false, Qt::DisplayRole);
        setChild(3, 0, roItem);
    }

    void setName(const QString &name)
    {
        child(0, 0)->setData(name, Qt::DisplayRole);
        setText(name);
    }

    QString name() const
    {
        return child(0, 0)->data(Qt::DisplayRole).toString();
    }

    void setDefaultValue(const QVariant &value)
    {
        child(1, 0)->setData(value, Qt::DisplayRole);
    }

    QString defaultValue() const
    {
        return child(1, 0)->data(Qt::DisplayRole).toString();
    }

    int valueType() const
    {
        return child(2, 0)->data(Qt::DisplayRole).toInt();
    }

    void setValueType(int type)
    {
        child(2, 0)->setData(type, Qt::DisplayRole);
    }

    bool isReadOnly() const
    {
        return child(3, 0)->data(Qt::DisplayRole).toBool();
    }

    void setReadOnly(bool readOnly)
    {
        child(3, 0)->setData(readOnly);
    }
};

class EntityItem: public IDocumentItem
{
public:
    EntityItem():
        IDocumentItem()
    {
        setColumnCount(1);
        setRowCount(3);
        auto name = new QStandardItem();
        setChild(0, 0, name);
        auto ports = new QStandardItem();
        setChild(1, 0, ports);
        generics->setText("Ports");
        auto generics = new QStandardItem();
        setChild(2, 0, generics);
        generics->setText("Generics");
    }

    QString name() const
    {
        return child(0, 0)->data(Qt::DisplayRole).toString();
    }

    void setName(const QString &name)
    {
        child(0, 0)->setData(name, Qt::DisplayRole);
        setText(name);
    }

    void addGeneric(GenericItem *generic)
    {
        child(2, 0)->appendRow(generic);
    }
};

class PackageItem: public IDocumentItem
{
public:
    explicit PackageItem()
    {
        setColumnCount(1);
        setRowCount(2);
        auto name = new QStandardItem();
        name->setData("", Qt::DisplayRole);
        setChild(0, 0, name);
        auto entities = new QStandardItem();
        setChild(1, 0, entities);
        entities->setText("Entities");
    }

    ~PackageItem()
    {
    }

    QString name() const
    {
        return child(0, 0)->data(Qt::DisplayRole).toString();
    }

    void setName(const QString &name)
    {
        child(0, 0)->setData(name, Qt::DisplayRole);
        setText(name);
    }

    void addEntity(EntityItem *entity)
    {
        child(1, 0)->appendRow(entity);
    }

    virtual int type() const
    {
        return IDocumentItem::PackageItemType;
    }
};

class LibraryItem: public IDocumentItem
{

public:
    explicit LibraryItem():
        IDocumentItem()
    {
        setColumnCount(1);
        setRowCount(3);
        auto name = new QStandardItem();
        name->setData("", Qt::DisplayRole);
        setChild(0, 0, name);
        auto author = new QStandardItem();
        author->setData("", Qt::DisplayRole);
        setChild(1, 0, author);
        auto packages = new QStandardItem();
        packages->setData("Packages", Qt::DisplayRole);
        setChild(2, 0, packages);
    }

    ~LibraryItem()
    {
    }

    // Library item specifics
    QString name() const
    {
        return child(0, 0)->data(Qt::DisplayRole).toString();
    }

    void setName(const QString &name)
    {
        child(0, 0)->setData(name, Qt::DisplayRole);
        setText(name);
    }

    QString author() const
    {
        return child(1,0)->data(Qt::DisplayRole).toString();
    }

    void setAuthor(const QString &author)
    {
        child(1, 0)->setData(author, Qt::DisplayRole);
    }

    void addPackage(PackageItem *package)
    {
        child(2, 0)->appendRow(package);
    }

    virtual int type() const
    {
        return IDocumentItem::LibraryItemType;
    }
};

#endif // DOCUMENTITEM_H
