#include "generic.h"
namespace entity
{

bool Generic::isValid() const
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

    if (type == QMetaType::UnknownType) {
        m_validity = false;
        m_errorMessage = "Type cannot be empty";
        return;
    }

    if (!required && defaultValue.isNull()) {
        m_validity = false;
        m_errorMessage = "Missing default value";
        return;
    }

    if (!defaultValue.isValid()) {
        m_validity = false;
        m_errorMessage = "Invalid default value";
        return;
    }
}

QString Generic::errorMessage() const
{
    return m_errorMessage;
}

}
