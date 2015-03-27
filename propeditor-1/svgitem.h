#ifndef SVGITEM_H
#define SVGITEM_H

#include "GraphicsObject.h"
#include <QGraphicsSvgItem>

class SvgItem: public DrawingItem<QGraphicsSvgItem>
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    QString m_fileName;

public:
    SvgItem(QGraphicsItem *parent = 0);
    ~SvgItem() {}

    QString fileName() const;

public slots:
    void setFileName(QString arg);

signals:
    void itemChanged();
    void fileNameChanged(QString arg);

private:
};

#endif // SVGITEM_H
