#ifndef KDEPLUGIN1_H
#define KDEPLUGIN1_H

#include "kdeplugin1_global.h"

#include "plugin.h"

class KDEPLUGIN1SHARED_EXPORT Kdeplugin1: public Plugin
{
    Q_OBJECT
public:
    Kdeplugin1(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    virtual ~Kdeplugin1();

    KTextEdit *createEditor();

private:
};

K_PLUGIN_FACTORY_DECLARATION(Kdeplugin1Factory)
#endif // KDEPLUGIN1_H
