#include "actionhandler.h"

#include <QAction>

ActionHandler::ActionHandler(QAction *action, QObject *parent) :
    QObject(parent),
    m_action(action),
    m_triggeringEnabled(false),
    m_togglingEnabled(false)
{
}

ActionHandler::~ActionHandler()
{

}

QAction *ActionHandler::action()
{
    return m_action;
}

void ActionHandler::enableTriggering(bool enabled)
{
    if (m_triggeringEnabled == enabled)
        return;

    if (m_triggeringEnabled) {
        disconnect(m_action, SIGNAL(triggered(bool)),
                   this, SLOT(onActionTriggered(bool)));
    }
    else {
        connect(m_action, SIGNAL(triggered(bool)),
                this, SLOT(onActionTriggered(bool)));
    }

    m_triggeringEnabled = !m_triggeringEnabled;
}

void ActionHandler::enableToggling(bool enabled)
{
    if (m_togglingEnabled == enabled)
        return;

    if (m_togglingEnabled) {
        disconnect(m_action, SIGNAL(toggled(bool)),
                   this, SLOT(onActionToggled(bool)));
    }
    else {
        connect(m_action, SIGNAL(toggled(bool)),
                this, SLOT(onActionToggled(bool)));
    }

    m_togglingEnabled = !m_togglingEnabled;
}


CommandActionBridge::CommandActionBridge(QAction *action, QUndoCommand *command, QObject *parent):
    ActionHandler(action, parent),
    m_command(command)
{

}

CommandActionBridge::~CommandActionBridge()
{

}

void CommandActionBridge::onActionTriggered(bool checked)
{

}

void CommandActionBridge::onActionToggled(bool checked)
{

}
