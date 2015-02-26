#ifndef MESSAGELOGVIEWER_H
#define MESSAGELOGVIEWER_H

#include <QFrame>

namespace Ui {
class MessageLogViewer;
}

class LoggingModel;

class MessageLogViewer : public QFrame
{
    Q_OBJECT

public:
    explicit MessageLogViewer(QWidget *parent = 0);
    ~MessageLogViewer();

    LoggingModel *model();
    void setModel(LoggingModel *model);

private:
    Ui::MessageLogViewer *ui;
    LoggingModel *m_model;
};

#endif // MESSAGELOGVIEWER_H
