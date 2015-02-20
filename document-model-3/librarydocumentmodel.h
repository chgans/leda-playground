#ifndef LIBRARYDOCUMENTMODEL_H
#define LIBRARYDOCUMENTMODEL_H

#include <QStandardItemModel>

#include "documentitem.h"
#include "librarydocumentmodel.h"

class DocumentModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DocumentModel(QObject *parent = 0):
        QStandardItemModel(parent)
    {

    }

    void setDocumentItem(IDocumentItem *item)
    {
        beginResetModel();
        appendRow(item);
        endResetModel();
    }

private:
    IDocumentItem *m_item;
};

#endif // LIBRARYDOCUMENTMODEL_H
