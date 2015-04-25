#ifndef KDEPLUGIN1_H
#define KDEPLUGIN1_H

#include "core/plugin.h"
#include <KTextEdit>

class TextEditorPlugin: public IEditorPlugin
{
    Q_OBJECT
    Q_PROPERTY(QString dummy2 READ dummy2)
public:
    TextEditorPlugin(QObject *parent, const QVariantList &args);
    virtual ~TextEditorPlugin();
    QString dummy2() const;

public slots:
    KTextEdit *createEditor();
};

#endif // KDEPLUGIN1_H
