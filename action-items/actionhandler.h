#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

class QAction;
class QUndoCommand;

class ActionHandler : public QObject
{
    Q_OBJECT

public:
    explicit ActionHandler(QAction *action, QObject *parent = 0);
    virtual ~ActionHandler();

    QAction *action();

signals:

protected slots:
    virtual void onActionTriggered(bool checked) = 0;
    virtual void onActionToggled(bool checked) = 0;

protected:
    void enableTriggering(bool enabled);
    void enableToggling(bool enabled);

private:
    QAction *m_action;
    bool m_triggeringEnabled;
    bool m_togglingEnabled;
};


class CommandActionBridge: public ActionHandler
{
    Q_OBJECT

public:
    explicit CommandActionBridge(QAction *action, QUndoCommand *command,
                                 QObject *parent = 0);
    virtual ~CommandActionBridge();

protected slots:
    void onActionTriggered(bool checked);
    void onActionToggled(bool checked);

private:
    QUndoCommand *m_command;
};

#endif // ACTIONHANDLER_H
