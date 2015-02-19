#ifndef TEXTFRAMEITEM_H
#define TEXTFRAMEITEM_H

#include "GraphicsObject.h"
#include <QGraphicsTextItem>

class TextFrameItem: public DrawingItem<QGraphicsTextItem>
{
    Q_OBJECT

    Q_PROPERTY(int textWidth READ textWidth WRITE setTextWidth)

public:
    TextFrameItem(QGraphicsItem *parent = 0);

public slots:

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void itemChanged();
    void flagsChanged();
    void focusChanged(bool focused);
    void cursorPositionChanged();
};

#endif // TEXTFRAMEITEM_H
