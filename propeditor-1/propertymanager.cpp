#include "propertymanager.h"
#include "qtpropertymanager.h"

#include <QDebug>

#define DBG() qDebug() << Q_FUNC_INFO
#define DBG2() DBG() << (property ? property->propertyName() : "null")

class FilePathPropertyType {};

Q_DECLARE_METATYPE(FilePathPropertyType)

PropertyManager::PropertyManager(QObject *parent) :
    QtVariantPropertyManager(parent)
{
    DBG();
    m_typeToPropertyManager[filePathTypeId()] = new QtStringPropertyManager; // FIXME
    m_typeToValueType[filePathTypeId()] = QVariant::String;
}

PropertyManager::~PropertyManager()
{
    DBG();
    clear();
}

QtVariantProperty *PropertyManager::addProperty(int propertyType, const QString &name)
{
    DBG() << name;
    if (propertyType == QVariant::String && name.endsWith("filename", Qt::CaseInsensitive)) {
        bool wasCreating = m_creatingProperty;
        m_creatingProperty = true;
        m_propertyType = filePathTypeId();
        QtProperty *property = QtVariantPropertyManager::addProperty(propertyType, name);
        m_creatingProperty = wasCreating;
        m_propertyType = 0;

        if (!property)
            return 0;

        return variantProperty(property);
    }
    else
        return QtVariantPropertyManager::addProperty(propertyType, name);
}

int PropertyManager::propertyType(const QtProperty *property) const
{
    DBG();
    return QtVariantPropertyManager::propertyType(property);
}

int PropertyManager::valueType(const QtProperty *property) const
{
    DBG();
    return QtVariantPropertyManager::valueType(property);
}

QtVariantProperty *PropertyManager::variantProperty(const QtProperty *property) const
{
    DBG();
    return QtVariantPropertyManager::variantProperty(property);
}

bool PropertyManager::isPropertyTypeSupported(int propertyType) const
{
    DBG();
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int PropertyManager::valueType(int propertyType) const
{
    DBG();
    return QtVariantPropertyManager::valueType(propertyType);
}

QVariant PropertyManager::value(const QtProperty *property) const
{
    DBG2();
    return QtVariantPropertyManager::value(property);
}

int PropertyManager::filePathTypeId()
{
    return qMetaTypeId<FilePathPropertyType>();
}

void PropertyManager::setValue(QtProperty *property, const QVariant &val)
{
    DBG2() << val;
    QtVariantPropertyManager::setValue(property, val);
}

void PropertyManager::setAttribute(QtProperty *property, const QString &attribute, const QVariant &value)
{
    DBG2();
    QtVariantPropertyManager::setAttribute(property, attribute, value);
}

bool PropertyManager::hasValue(const QtProperty *property) const
{
    DBG2();
    return QtVariantPropertyManager::hasValue(property);
}

QString PropertyManager::valueText(const QtProperty *property) const
{
    DBG2();
    return QtVariantPropertyManager::valueText(property);
}

QIcon PropertyManager::valueIcon(const QtProperty *property) const
{
    DBG2();
    return QtVariantPropertyManager::valueIcon(property);
}

void PropertyManager::initializeProperty(QtProperty *property)
{
    DBG2();
    QtVariantPropertyManager::initializeProperty(property);
}

void PropertyManager::uninitializeProperty(QtProperty *property)
{
    DBG2();
    QtVariantPropertyManager::uninitializeProperty(property);
}

QtProperty *PropertyManager::createProperty()
{
    DBG();
    if (!m_creatingProperty)
        return QtVariantPropertyManager::createProperty();
    DBG() << "Creating filepath";
    Property *property = new Property(this);
    m_propertyToType.insert(property, qMakePair(property, m_propertyType));

    return property;
}


Property::~Property()
{

}

Property::Property(PropertyManager *manager):
    QtVariantProperty(manager)
{

}


PropertyEditorFactory::PropertyEditorFactory(QObject *parent):
    QtAbstractEditorFactory(parent)
{

}

PropertyEditorFactory::~PropertyEditorFactory()
{

}

void PropertyEditorFactory::connectPropertyManager(PropertyManager *manager)
{

}

#include <QHBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QToolButton>

QWidget *PropertyEditorFactory::createEditor(PropertyManager *manager, QtProperty *property, QWidget *parent)
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *l = new QHBoxLayout;
    w->setLayout(l);
    l->addWidget(new QLineEdit);
    l->addWidget(new QToolButton);
    return w;
}

void PropertyEditorFactory::disconnectPropertyManager(PropertyManager *manager)
{

}

void PropertyEditorFactory::slotPropertyChanged(QtProperty *, const QString &)
{

}

void PropertyEditorFactory::slotSetValue(const QString &)
{

}

void PropertyEditorFactory::slotEditorDestroyed(QObject *)
{

}
