#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QVariantList>

#include "core_global.h"

class KTextEdit;

class IEditorPlugin: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dummy READ dummy)

public:
    IEditorPlugin(QObject *parent):
        QObject(parent)
    {}
    virtual ~IEditorPlugin()
    {}

    QString dummy() const { return QStringLiteral("dummy"); }

public slots:
    virtual KTextEdit *createEditor() = 0;
};

#endif // PLUGIN_H



