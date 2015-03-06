#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = 0);
    ~Document();

signals:

public slots:
};

#endif // DOCUMENT_H
