#ifndef DOCUMENTFACTORY_H
#define DOCUMENTFACTORY_H

#include "abstractfactory.h"
#include "document.h"
#include "documentcreator.h"

class DocumentFactory : public AbstractFactory<Document, DocumentCreator>
{
public:
    ~DocumentFactory();

private:
    DocumentFactory();
};

#endif // DOCUMENTFACTORY_H
