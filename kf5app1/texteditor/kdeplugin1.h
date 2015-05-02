#ifndef KDEPLUGIN1_H
#define KDEPLUGIN1_H

#include "core/plugin.h"
#include <KTextEdit>

class TextEditorPlugin: public IEditorPlugin
{
    Q_OBJECT
public:
    TextEditorPlugin(QObject *parent, const QVariantList &args);
    virtual ~TextEditorPlugin();

public slots:
    KTextEditor *createEditor();
};

#endif // KDEPLUGIN1_H
