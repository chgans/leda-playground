#ifndef MESSAGELOGVIEWER_H
#define MESSAGELOGVIEWER_H

#include <QFrame>

namespace Ui {
class MessageLogViewer;
}

class MessageLogViewer : public QFrame
{
    Q_OBJECT

public:
    explicit MessageLogViewer(QWidget *parent = 0);
    ~MessageLogViewer();

private:
    Ui::MessageLogViewer *ui;
};

#endif // MESSAGELOGVIEWER_H
