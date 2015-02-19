#include "textframeitem.h"

TextFrameItem::TextFrameItem(QGraphicsItem *parent):
    DrawingItem(parent)
{

}

void TextFrameItem::focusInEvent(QFocusEvent *event)
{
    DrawingItem<QGraphicsTextItem>::focusInEvent(event);
    emit focusChanged(true);
}

void TextFrameItem::focusOutEvent(QFocusEvent *event)
{
    DrawingItem<QGraphicsTextItem>::focusOutEvent(event);
    emit focusChanged(false);
}

void TextFrameItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
    emit cursorPositionChanged();
}

void TextFrameItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
    emit cursorPositionChanged();
}

QVariant TextFrameItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        if (isSelected()) {
            setTextInteractionFlags(Qt::TextEditorInteraction);
            setCursor(QCursor(Qt::IBeamCursor));
        }
        else {
            setTextInteractionFlags(Qt::NoTextInteraction);
            unsetCursor();
        }
    }
    return DrawingItem<QGraphicsTextItem>::itemChange(change, value);
}
