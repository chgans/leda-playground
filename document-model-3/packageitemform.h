#ifndef PACKAGEITEMFORM_H
#define PACKAGEITEMFORM_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class PackageItemForm;
}

class PackageItem;
class DocumentModel;
class QDataWidgetMapper;
class PackageItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit PackageItemForm(QWidget *parent = 0);
    ~PackageItemForm();

    void setModel(DocumentModel *model);
    void setRootModelIndex(const QModelIndex &index);

protected slots:
    void nameChanged(const QString &name);

private:
    Ui::PackageItemForm *ui;
    QDataWidgetMapper *m_mapper;
    DocumentModel *m_model;
    QModelIndex m_rootIndex;
};

#endif // PACKAGEITEMFORM_H
