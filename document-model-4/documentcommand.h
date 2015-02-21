#ifndef DOCUMENTTRANSACTION_H
#define DOCUMENTTRANSACTION_H

#include <QUndoCommand>
#include <QPersistentModelIndex>
#include <QVariant>

class DocumentModel;
class DocumentObject;

class DocumentCommand: public QUndoCommand
{
public:
    enum Id {
        AddObject = 0,
        RemoveObject,
        AddProperty,
        RemoveProperty,
        ChangePropertyValue,
        ChangePropertyName,
    };

    DocumentCommand(DocumentCommand *parent = nullptr);
    virtual ~DocumentCommand();

    void setModel(DocumentModel *model);
    DocumentModel *model() const;

    void setIndex(const QPersistentModelIndex &index);
    QPersistentModelIndex index();

private:
    DocumentModel *m_model;
    QPersistentModelIndex m_index;
};

class AddPropertyCommand: public DocumentCommand
{
public:
    AddPropertyCommand(DocumentCommand *parent = nullptr);
    virtual ~AddPropertyCommand();

    void setPropertyName(const QString &name);
    QString propertyName() const;
    void setPropertyValue(const QVariant &value);
    QVariant propertyValue() const;

    bool mergeWith(const DocumentCommand *other);
    int id() const;
    void undo();
    void redo();

private:
    DocumentObject *m_object;
    QString m_name;
    QVariant m_value;
    QPersistentModelIndex m_index;
    int m_column;
};


class ChangePropertyValueCommand: public DocumentCommand
{
public:
    ChangePropertyValueCommand(DocumentCommand *parent = nullptr);
    virtual ~ChangePropertyValueCommand();

    void setPropertyName(const QString &name);
    QString propertyName() const;
    void setPropertyValue(const QVariant &current, const QVariant &previous);
    QVariant propertyValue() const;

    bool mergeWith(const DocumentCommand *other);
    int id() const;
    void undo();
    void redo();

private:
    DocumentObject *m_object;
    QString m_name;
    QVariant m_currentValue;
    QVariant m_previousValue;
    QPersistentModelIndex m_index;
    int m_column;
};


class RemovePropertyCommand: public DocumentCommand
{
public:
    RemovePropertyCommand(DocumentCommand *parent = nullptr);
    virtual ~RemovePropertyCommand();

    void setPropertyName(const QString &name);
    QString propertyName() const;
    void setPropertyValue(const QVariant &value);
    QVariant propertyValue() const;

    virtual bool mergeWith(const DocumentCommand *other);
    virtual int id() const;
    virtual void undo();
    virtual void redo();

private:
    QString m_name;
    QVariant m_value;
};


class AddObjectCommand: public DocumentCommand
{
public:
    AddObjectCommand(DocumentCommand *parent = nullptr);
    virtual ~AddObjectCommand();

    virtual bool mergeWith(const DocumentCommand *other);
    virtual int id() const;
    virtual void undo();
    virtual void redo();

    void setObject(DocumentObject *object);
    DocumentObject *object();

private:
    DocumentObject *m_object;
};

class RemoveObjectCommand: public DocumentCommand
{
public:
    RemoveObjectCommand(DocumentCommand *parent = nullptr);
    virtual ~RemoveObjectCommand();

    virtual bool mergeWith(const DocumentCommand *other);
    virtual int id() const;
    virtual void undo();
    virtual void redo();

    void setObject(DocumentObject *object);
    DocumentObject *object();

private:
    DocumentObject *m_object;
};
#endif // DOCUMENTTRANSACTION_H
