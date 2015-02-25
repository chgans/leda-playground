#include "documentobjectpropertybrowserwidget.h"
#include "documentobject.h"
#include "documentmodel.h"

#include "3rdParty/qtpropertybrowser/qttreepropertybrowser.h"
#include "3rdParty/qtpropertybrowser/qtvariantproperty.h"

#include <QMetaProperty>
#include <QVBoxLayout>

#include <QDebug>

DocumentObjectPropertyBrowserWidget::DocumentObjectPropertyBrowserWidget(QWidget *parent) :
    QWidget(parent), m_model(nullptr), m_object(nullptr), m_updating(false),
    m_browser(new QtTreePropertyBrowser(this)),
    m_manager(new QtVariantPropertyManager(this)),
    m_factory(new QtVariantEditorFactory(this))
{
    m_browser->setRootIsDecorated(true);
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
    qDebug() << "DOPB::setObjectModelIndex" << index << m_model->object(index)->objectName();
    m_objectIndex = index;
    m_object = m_model->object(index);
    updateBrowser();
}

// TODO: don't assume QObject, query the model instead
void DocumentObjectPropertyBrowserWidget::populateBrowser(QObject *object, const QMetaObject *metaObject)
{
    if (!object || !metaObject)
        return;

    populateBrowser(object, metaObject->superClass());
    QtProperty *propGroup = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                   metaObject->className());
    for (int i=0; i<metaObject->propertyCount(); ++i) {
        QMetaProperty metaProp = metaObject->property(i);
        if (metaProp.enclosingMetaObject() != metaObject) {
            continue;
        }
        QtProperty *prop = m_manager->addProperty(metaProp.type(),
                                                  metaProp.name());
        m_qtPropToColumn[prop] = i;
        m_columnToQtProp[i] = prop;
        m_manager->setValue(prop, object->property(metaProp.name()));
        if (!prop) {
            qDebug() << QString("Cannot add property %1:%2 of type %3").arg(metaObject->className()).arg(metaProp.name()).arg(metaProp.typeName());
            continue;
        }
        prop->setEnabled(metaObject->property(i).isWritable());
        propGroup->addSubProperty(prop);
    }
    QtBrowserItem *item = m_browser->addProperty(propGroup);
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

    m_columnToQtProp.clear();
    m_qtPropToColumn.clear();
    m_updating = true;
    populateBrowser(m_object, m_object->metaObject());
    m_updating = false;
}

void DocumentObjectPropertyBrowserWidget::onValueChanged(QtProperty *property, QVariant value)
{
    if (m_updating)
        return;

    Q_ASSERT(m_qtPropToColumn.contains(property));

    int column = m_qtPropToColumn[property];
    QModelIndex propertyIndex = m_model->index(m_objectIndex.row(), column, m_objectIndex.parent());
    QVariant previous = m_model->data(propertyIndex, Qt::DisplayRole);

    if (previous == value)
        return;

    qDebug() << "onValueChanged" << property->propertyName() << previous << "=>" << value;
    qDebug() << "onValueChanged" << propertyIndex;
    emit valueChanged(propertyIndex, property, value, previous);
}

void DocumentObjectPropertyBrowserWidget::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);

    QModelIndex index(topLeft);

    if (m_model->object(index) != m_object)
        return;

    DocumentObject *object = m_model->object(index);
    int propertyColumn = index.column();
    QtProperty *qtProperty = m_columnToQtProp[propertyColumn];
    QVariant value = object->data(propertyColumn);

    qDebug() << "onModelDataChanged" << index << m_model->object(index)->objectName() << value;

    if (value == m_manager->value(qtProperty))
        return;

    //m_updating = true;
    m_manager->setValue(qtProperty, value);
    //m_updating = false;
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
