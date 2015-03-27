#ifndef LABELITEM_H
#define LABELITEM_H

#include "GraphicsObject.h"
#include <QGraphicsTextItem>


class LabelItem: public DrawingItem<QGraphicsTextItem>
{
    Q_OBJECT

    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    LabelItem(QGraphicsItem *parent = 0);
    ~LabelItem();

    QString text() const;

public slots:
    void setText(const QString &arg);
    void setFont(const QFont &arg);

signals:
    void itemChanged();
    void textChanged(const QString &arg);
    void fontChanged(const QFont &arg);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // LABELITEM_H
