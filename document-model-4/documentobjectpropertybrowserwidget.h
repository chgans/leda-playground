#ifndef DOCUMENTOBJECTPROPERTYBROWSERWIDGET_H
#define DOCUMENTOBJECTPROPERTYBROWSERWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QPersistentModelIndex>

class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtProperty;

class DocumentObject;
class DocumentModel;

class DocumentObjectPropertyBrowserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentObjectPropertyBrowserWidget(QWidget *parent = 0);

    void setDocumentModel(DocumentModel *model);
    void setObjectModelIndex(const QPersistentModelIndex &index);

signals:
    void valueChanged(const QPersistentModelIndex &index, QtProperty *property,
                      const QVariant &currentValue, const QVariant &previousValue);

public slots:

protected slots:
    void populateBrowser(QObject *object, const QMetaObject *metaObject);
    void updateBrowser();
    void onValueChanged(QtProperty *property, QVariant value);
    //void onPropertyAdded(QtProperty *property, QVariant value);
    void onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
    void connectModel();
    void disconnectModel();

private:
    DocumentModel *m_model;
    DocumentObject *m_object;
    QPersistentModelIndex m_objectIndex;
    QPersistentModelIndex m_propertyIndex;
    bool m_populating;
    QtTreePropertyBrowser *m_browser;
    QtVariantPropertyManager *m_manager;
    QtVariantEditorFactory *m_factory;
};

#endif // DOCUMENTOBJECTPROPERTYBROWSERWIDGET_H
