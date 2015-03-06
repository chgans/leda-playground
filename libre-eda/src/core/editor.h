#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>

class Editor : public QObject
{
    Q_OBJECT
public:
    explicit Editor(QObject *parent = 0);
    ~Editor();

signals:

public slots:
};

#endif // EDITOR_H
