#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include "qtvariantproperty.h"

class PropertyManager;

class Property: public QtVariantProperty
{
public:
    ~Property();

protected:
    Property(PropertyManager *manager);
private:
    friend class PropertyManager;
};

class QtStringPropertyManager;

class PropertyEditorFactory : public QtAbstractEditorFactory<PropertyManager>
{
    Q_OBJECT
public:
    PropertyEditorFactory(QObject *parent = 0);
    ~PropertyEditorFactory();
protected:
    void connectPropertyManager(PropertyManager *manager);
    QWidget *createEditor(PropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(PropertyManager *manager);
private:
    Q_DISABLE_COPY(PropertyEditorFactory)
private slots:
    void slotPropertyChanged(QtProperty *, const QString &);
    //void slotRegExpChanged(QtProperty *, const QRegExp &);
    void slotSetValue(const QString &);
    void slotEditorDestroyed(QObject *);
};


class PropertyManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    explicit PropertyManager(QObject *parent = 0);
    ~PropertyManager();

    QtVariantProperty *addProperty(int propertyType, const QString &name = QString());

    int propertyType(const QtProperty *property) const;
    int valueType(const QtProperty *property) const;
    QtVariantProperty *variantProperty(const QtProperty *property) const;

    virtual bool isPropertyTypeSupported(int propertyType) const;
    virtual int valueType(int propertyType) const;

    virtual QVariant value(const QtProperty *property) const;

    static int filePathTypeId();

signals:

public slots:
    virtual void setValue(QtProperty *property, const QVariant &val);
    virtual void setAttribute(QtProperty *property,
                              const QString &attribute, const QVariant &value);

protected:
    virtual bool hasValue(const QtProperty *property) const;
    QString valueText(const QtProperty *property) const;
    QIcon valueIcon(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
    virtual QtProperty *createProperty();

private:
    bool m_creatingProperty;
    int m_propertyType;

    QMap<int, int> m_typeToValueType;
    QMap<int, QtAbstractPropertyManager *> m_typeToPropertyManager;
    QMap<const QtProperty *, QPair<QtVariantProperty *, int> > m_propertyToType;
};

#endif // PROPERTYMANAGER_H
