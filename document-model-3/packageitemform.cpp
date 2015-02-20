#include "packageitemform.h"
#include "ui_packageitemform.h"
#include "documentitem.h"
#include "librarydocumentmodel.h"

#include <QDataWidgetMapper>

PackageItemForm::PackageItemForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PackageItemForm)
{
    ui->setupUi(this);
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    connect(ui->nameValue, SIGNAL(textChanged(QString)),
            this, SLOT(nameChanged(QString)));
}

PackageItemForm::~PackageItemForm()
{
    delete ui;
}


void PackageItemForm::setModel(DocumentModel *model)
{
    m_model = model;
}

void PackageItemForm::setRootModelIndex(const QModelIndex &index)
{
    m_rootIndex = index;
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->setModel(m_model);
    m_mapper->setRootIndex(index);
    m_mapper->addMapping(ui->nameValue, 0);
    m_mapper->addMapping(ui->displayValue, 1);
    m_mapper->addMapping(ui->authorValue, 2);
    m_mapper->addMapping(ui->licenseValue, 3);
    m_mapper->setCurrentIndex(0);
}

void PackageItemForm::nameChanged(const QString &name)
{

}
