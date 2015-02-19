#ifndef LIBRARYDOCUMENTMODEL_H
#define LIBRARYDOCUMENTMODEL_H

#include <QStandardItemModel>

#include "documentitem.h"
#include "librarydocumentmodel.h"

class LibraryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit LibraryModel(QObject *parent = 0):
        QStandardItemModel(parent)
    {

    }

    void setLibrary(LibraryItem *library)
    {
        beginResetModel();
        QList<QStandardItem *> list;
        list.append(library);
        appendRow(list);
        endResetModel();
    }

private:
    LibraryItem *m_library;
};

#endif // LIBRARYDOCUMENTMODEL_H
