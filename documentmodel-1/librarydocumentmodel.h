#ifndef LIBRARYDOCUMENTMODEL_H
#define LIBRARYDOCUMENTMODEL_H

#include <QAbstractItemModel>

class LibraryDocumentModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LibraryDocumentModel(QObject *parent = 0);

signals:

public slots:

};

#endif // LIBRARYDOCUMENTMODEL_H
