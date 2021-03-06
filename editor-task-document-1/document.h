#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

class DocumentItem;

// TODO:
//   autosave stuff
//   load/save error
class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isModified READ isModified)
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)

public:
    explicit Document(QObject *parent = 0);
    ~Document();

    void load(const QString &filename);
    void save();

    bool isModified() const;
    QString filename() const;

signals:
    void aboutToBeModified() const;
    void wasModified() const;
    void filenameChanged() const;

public slots:

private:
    QString m_filename;
    DocumentItem *m_root;
};

#endif // DOCUMENT_H
