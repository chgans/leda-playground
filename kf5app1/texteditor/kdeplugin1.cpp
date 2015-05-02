
#include "kdeplugin1.h"
#include "core/plugin.h"

#include <KPluginFactory>

TextEditorPlugin::TextEditorPlugin(QObject *parent, const QVariantList &/*args*/):
    IEditorPlugin(parent)
{}

TextEditorPlugin::~TextEditorPlugin()
{}

QString TextEditorPlugin::dummy2() const
{
    return QStringLiteral("dummy");
}

KTextEditor *TextEditorPlugin::createEditor()
{
    return new KTextEdit();
}

