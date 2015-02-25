#include "documentcommand.h"
#include "documentmodel.h"
#include "documentobject.h"

#include <QDebug>

DocumentCommand::DocumentCommand(DocumentCommand *parent):
    QUndoCommand(parent)
{
}

DocumentCommand::~DocumentCommand()
{

}

void DocumentCommand::setModel(DocumentModel *model)
{
    m_model = model;
}

DocumentModel *DocumentCommand::model() const
{
    return m_model;
}

void DocumentCommand::setIndex(const QPersistentModelIndex &index)
{
    m_index = index;
}

QPersistentModelIndex DocumentCommand::index()
{
    return m_index;
}


AddPropertyCommand::AddPropertyCommand(DocumentCommand *parent):
    DocumentCommand(parent)
{

}

AddPropertyCommand::~AddPropertyCommand()
{
    setText("Add property");
}

void AddPropertyCommand::setPropertyName(const QString &name)
{
    m_name = name;
}

QString AddPropertyCommand::propertyName() const
{
    return m_name;
}

void AddPropertyCommand::setPropertyValue(const QVariant &value)
{
    m_value = value;
}

QVariant AddPropertyCommand::propertyValue() const
{
    return m_value;
}

bool AddPropertyCommand::mergeWith(const DocumentCommand *other)
{
    return false;
}

int AddPropertyCommand::id() const
{
    return DocumentCommand::AddProperty;
}

void AddPropertyCommand::undo()
{
    model()->removeProperty(m_index);
}

void AddPropertyCommand::redo()
{
    m_index = model()->addProperty(index(), m_name, m_value);
}


ChangePropertyValueCommand::ChangePropertyValueCommand(DocumentCommand *parent):
    DocumentCommand(parent)
{
    setText("Change property");
}

ChangePropertyValueCommand::~ChangePropertyValueCommand()
{

}

void ChangePropertyValueCommand::setPropertyName(const QString &name)
{
    m_name = name;
}

QString ChangePropertyValueCommand::propertyName() const
{
    return m_name;
}

void ChangePropertyValueCommand::setPropertyValue(const QVariant &current, const QVariant &previous)
{
    m_currentValue = current;
    m_previousValue = previous;
}

QVariant ChangePropertyValueCommand::propertyValue() const
{
    return m_currentValue;
}

bool ChangePropertyValueCommand::mergeWith(const DocumentCommand *other)
{
    qDebug() << id() << other->id();
    if (id() != other->id())
        return false;
    const ChangePropertyValueCommand *command = static_cast<const ChangePropertyValueCommand*>(other);
    m_previousValue = command->m_previousValue;
    return true;
}

int ChangePropertyValueCommand::id() const
{
    return DocumentCommand::ChangePropertyValue;
}

void ChangePropertyValueCommand::undo()
{
    qDebug() << "undo" << index() << m_currentValue << "=>" << m_previousValue;
    model()->changeProperty(index(), m_previousValue);
}

void ChangePropertyValueCommand::redo()
{
    qDebug() << "redo" << index() << m_previousValue << "=>" << m_currentValue;
    model()->changeProperty(index(), m_currentValue);
}
