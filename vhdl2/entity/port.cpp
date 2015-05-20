#include "port.h"

namespace entity
{

bool Port::isValid() const
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

}

QString Port::errorMessage() const
{
    return m_errorMessage;
}


}
