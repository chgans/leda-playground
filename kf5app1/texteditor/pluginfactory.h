#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

#include "kdeplugin1.h"
#include <KPluginFactory>

class TextEditorPluginFactory: public KPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(KPluginFactory)
    Q_PLUGIN_METADATA(IID KPluginFactory_iid FILE "texteditor.json")

public:
    explicit TextEditorPluginFactory()
    {
        registerPlugin<TextEditorPlugin>();
    }
    ~TextEditorPluginFactory() {}

};

//K_PLUGIN_FACTORY_WITH_JSON(MyPluginFactory,
//                           "texteditor.json",
//                           registerPlugin<TextEditorPlugin>();
//)

#endif // PLUGINFACTORY_H
