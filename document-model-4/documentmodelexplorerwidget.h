#ifndef DOCUMENTMODELEXPLORERWIDGET_H
#define DOCUMENTMODELEXPLORERWIDGET_H

#include <QWidget>
#include <QItemSelection>

class QTreeView;
class DocumentModel;
class DocumentObject;

class DocumentModelExplorerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentModelExplorerWidget(QWidget *parent = 0);

    void setModel(DocumentModel *model);
    DocumentModel *model();

signals:
    void selectionChanged(const QPersistentModelIndex &index);
public slots:

protected slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    QTreeView *m_tree;
    DocumentModel *m_model;
};

#endif // DOCUMENTMODELEXPLORERWIDGET_H
