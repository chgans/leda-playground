#ifndef DESIGNLAYERMANAGER_H
#define DESIGNLAYERMANAGER_H

#include <QObject>

class DesignLayerManager : public QObject
{
    Q_OBJECT
public:
    explicit DesignLayerManager(QObject *parent = 0);
    ~DesignLayerManager();

signals:

public slots:
};

#endif // DESIGNLAYERMANAGER_H
