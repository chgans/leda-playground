#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <KXMLGUIClient>

#include <KPluginFactory>
#include <KPluginLoader>

class KTextEdit;

class Plugin:
        public QObject,
        public KXMLGUIClient
{
    Q_OBJECT
public:
    Plugin(QObject *parent, const QVariantList &args):
        QObject(parent),
        KXMLGUIClient()
    {}
    virtual ~Plugin();

    virtual KTextEdit *createEditor() = 0;

    const QVariantList &args() const
    {
        return m_args;
    }

private:
    QVariantList m_args;
};

#endif // PLUGIN_H



