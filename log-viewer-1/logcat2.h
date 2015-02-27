#ifndef LOGCAT2_H
#define LOGCAT2_H

#include <QObject>
#include <QTimer>

class logcat2 : public QObject
{
    Q_OBJECT
public:
    explicit logcat2(QObject *parent = 0);
    ~logcat2();

signals:

public slots:
    void doIt();

private:
    QTimer m_timer;
};

#endif // LOGCAT2_H
