#include "kdeplugin1.h"

#include <KTextEdit>
#include <KLocalizedString>


K_PLUGIN_FACTORY_WITH_JSON(TextEditorFactory,
                           "texteditor.json",
                           registerPlugin<Kdeplugin1>();
)

Kdeplugin1::Kdeplugin1(QObject *parent, const QVariantList &args):
    Plugin(parent, args)
{

}

Kdeplugin1::~Kdeplugin1()
{

}

KTextEdit *Kdeplugin1::createEditor()
{
    return new KTextEdit();
}
