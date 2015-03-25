#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

#include <QVariantMap>

// See QJson{Document|Object}, QText{Document|Object|...}, QDom{Document|Element|...}
// See Freecad{Document|Object}

class DocumentObject;

class Document : public QObject
{
    Q_OBJECT

    // TODO: meta-data: author, date, ...
    // TODO: Don't inherit form QObject?
    // TODO: iterator stuff
    // TODO: Manage undo stack differently

    // TODO: properties
    // - modified
    // - objectCount

public:
    enum CommandStacks {
        UndoStack = 0x01,
        RedoStack = 0x02,
        UndoAndRedoStacks = 0x03
    };

    explicit Document(QObject *parent = 0);
    ~Document();
    // clone()
    // reference and/or link
    // operators =, !=, ...

    /* Load, save and modification */
public:
    virtual bool load();
    virtual bool save() const;
    bool isEmpty() const;
    bool isModified() const;

public slots:
    void setModified(bool modified = true);

signals:
    void loaded(bool success);
    void saved(bool success);
    void modificationChanged();

    /* Object */
public:
    void addObject(DocumentObject *obj);
    DocumentObject *addObject(const QString& typeName);
    void removeObject(DocumentObject *obj);
    DocumentObject *copyObject(DocumentObject *obj,
                               Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    DocumentObject *moveObject(DocumentObject *obj,
                               Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    template<T> findObject(const QString &name = QString(),
                           Qt::FindChildOptions options = Qt::FindChildrenRecursively) const;
    template<T> QList<T> findObjects(const QString &name = QString(),
                                     Qt::FindChildOptions options = Qt::FindChildrenRecursively) const;

    /* Undo/Redo support */
public:
    //void clearUndoRedoStacks(CommandStacks stacksToClear = UndoAndRedoStacks);
    //int availableUndoSteps() const;
    //int availableRedoSteps() const;
public slots:
    //void undo();
    //void redo();
signals:
    //void undoAvailable(const QString& label);
    //void redoAvailable(const QString& label);

public slots:

protected:
    QList<DocumentObject *> mObjects;
};

#endif // DOCUMENT_H
