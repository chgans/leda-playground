#ifndef SCHLIBRARYDOCKWIDGET_H
#define SCHLIBRARYDOCKWIDGET_H

#include <QDockWidget>
#include "documentmodels.h"
#include "documentitems.h"

namespace Ui {
class SchLibraryDockWidget;
}

class SchLibraryDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SchLibraryDockWidget(QWidget *parent = 0);
    ~SchLibraryDockWidget();

    void setSymbolCollection(DocumentSymbolCollectionItem *collection);

protected slots:
    void onSymbolSelectionChanged(const QModelIndex &current);

private:
    Ui::SchLibraryDockWidget *ui;
    DocumentSymbolCollectionItem *m_collectionItem;
    SymbolCollectionTableViewModel *m_collectionModel;
    DocumentParameterTableViewModel *m_paramModel;
};

#endif // SCHLIBRARYDOCKWIDGET_H
