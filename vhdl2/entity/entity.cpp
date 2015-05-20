#include "entity.h"

namespace entity
{

bool Entity::isValid() const
{
    if (m_validated)
        return m_validity;

    m_validity = true;
    m_errorMessage.clear();

    if (name.isEmpty()) {
        m_validity = false;
        m_errorMessage = "Name cannot be empty";
        return;
    }

    foreach (Generic *generic, generics) {
        if (!generic->isValid()) {
            m_validity = false;
            m_errorMessage = QString("Generic \"%1\": %2").arg(generic->name).arg(generic->errorMessage());
        }
    }

    foreach (Port *port, ports) {
        if (!port->isValid()) {
            m_validity = false;
            m_errorMessage = QString("Port \"%1\": %2").arg(port->name).arg(port->errorMessage());
        }
    }
}

QString Entity::errorMessage() const
{
    return m_errorMessage;
}



}
