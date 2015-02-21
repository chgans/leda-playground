#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>

class QUndoStack;

class DocumentModel;
class DocumentCommand;

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = 0);
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modificationChanged)

    QUrl url() const;
    void setUrl(const QUrl &url);
    bool open();
    bool save();
    bool saveAs(const QUrl &url);
    void close();
    void clear();

    bool isModified() const;
    bool isOpen() const;

    DocumentModel *model();
    QUndoStack *undoStack();
    void executeCommand(DocumentCommand *command);

    QString errorString();

signals:
    void modificationChanged(bool modified);
    void urlChanged(const QUrl &url);

private:
    QFile m_file;
    QJsonDocument m_document;
    bool m_isModified;
    DocumentModel *m_model;
    QString m_errorString;
    QUndoStack *m_undoStack;
};

#endif // DOCUMENT_H
