#ifndef LOGCAT1_H
#define LOGCAT1_H

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(log1)

class logcat1 : public QObject
{
    Q_OBJECT
public:
    explicit logcat1(QObject *parent = 0);
    ~logcat1();

signals:

public slots:
    void doIt();

private:
    QTimer m_timer;
};

#endif // LOGCAT1_H
