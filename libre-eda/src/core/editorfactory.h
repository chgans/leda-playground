#ifndef EDITORFACTORY_H
#define EDITORFACTORY_H

#include "abstractfactory.h"
#include "editor.h"
#include "editorcreator.h"

class EditorFactory : public AbstractFactory<Editor, EditorCreator>
{
public:
    ~EditorFactory();

private:
    EditorFactory();
};

#endif // EDITORFACTORY_H
