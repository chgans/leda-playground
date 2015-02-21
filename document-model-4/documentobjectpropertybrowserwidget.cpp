#include "documentobjectpropertybrowserwidget.h"
#include "documentobject.h"
#include "documentmodel.h"

#include "3rdParty/qtpropertybrowser/qttreepropertybrowser.h"
#include "3rdParty/qtpropertybrowser/qtvariantproperty.h"

#include <QMetaProperty>
#include <QVBoxLayout>

#include <QDebug>

DocumentObjectPropertyBrowserWidget::DocumentObjectPropertyBrowserWidget(QWidget *parent) :
    QWidget(parent), m_model(nullptr), m_object(nullptr), m_populating(false),
    m_browser(new QtTreePropertyBrowser(this)),
    m_manager(new QtVariantPropertyManager(this)),
    m_factory(new QtVariantEditorFactory(this))
{
    m_browser->setPropertiesWithoutValueMarked(true);
    m_browser->setFactoryForManager(m_manager, m_factory);
    connect(m_manager, SIGNAL(valueChanged(QtProperty*,QVariant)),
            this, SLOT(onValueChanged(QtProperty*,QVariant)));
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_browser);
}

void DocumentObjectPropertyBrowserWidget::setDocumentModel(DocumentModel *model)
{
    if (m_model)
        disconnectModel();
    m_model = model;
    connectModel();
}

void DocumentObjectPropertyBrowserWidget::setObjectModelIndex(const QPersistentModelIndex &index)
{
    m_objectIndex = index;
    m_object = m_model->object(index);
    updateBrowser();
}

void DocumentObjectPropertyBrowserWidget::populateBrowser(QObject *object, const QMetaObject *metaObject)
{
    if (!object || !metaObject)
        return;

    populateBrowser(object, metaObject->superClass());
    QtProperty *parentProp = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                   metaObject->className());
    for (int i=0; i<metaObject->propertyCount(); ++i) {
        QMetaProperty metaProp = metaObject->property(i);
        if (metaProp.enclosingMetaObject() != metaObject) {
            continue;
        }
        QtProperty *prop = m_manager->addProperty(metaProp.type(),
                                                  metaProp.name());
        m_manager->setValue(prop, object->property(metaProp.name()));
        if (!prop) {
            qDebug() << QString("Cannot add property %1:%2 of type %3").arg(metaObject->className()).arg(metaProp.name()).arg(metaProp.typeName());
            continue;
        }
        prop->setEnabled(metaObject->property(i).isWritable());
        parentProp->addSubProperty(prop);
    }
    QtBrowserItem *item = m_browser->addProperty(parentProp);
    foreach (QtBrowserItem *child, item->children()) {
        m_browser->setExpanded(child, false);
    }
}

void DocumentObjectPropertyBrowserWidget::updateBrowser()
{
    m_manager->clear();
    m_browser->clear();

    if (!m_object)
        return;

    m_populating = true;
    populateBrowser(m_object, m_object->metaObject());
    m_populating = false;
}

void DocumentObjectPropertyBrowserWidget::onValueChanged(QtProperty *property, QVariant value)
{
    if (m_populating)
        return;
    const char *propertyName = property->propertyName().toLocal8Bit().constData();
    int column = m_object->metaObject()->indexOfProperty(propertyName);
    QVariant previous = m_object->property(propertyName);
    m_propertyIndex = m_model->index(m_objectIndex.row(), column, m_objectIndex.parent());
    emit valueChanged(m_propertyIndex, property, value, previous);
    //qDebug() << property->propertyName() << previous << "=>" << value;
}

void DocumentObjectPropertyBrowserWidget::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << m_propertyIndex << QPersistentModelIndex(topLeft);
    if (m_propertyIndex != QPersistentModelIndex(topLeft))
        updateBrowser();
}

void DocumentObjectPropertyBrowserWidget::connectModel()
{
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(onModelDataChanged(QModelIndex,QModelIndex)));
}

void DocumentObjectPropertyBrowserWidget::disconnectModel()
{
    m_model->disconnect(this);
}
