#include "GraphicsObject.h"
#include "labelitem.h"


LabelItem::LabelItem(QGraphicsItem *parent):
    DrawingItem(parent)
{
}

LabelItem::~LabelItem() {}

QString LabelItem::text() const
{ return toPlainText(); }

void LabelItem::setText(const QString &arg)
{
    if (toPlainText() != arg) {
        QGraphicsTextItem::setPlainText(arg);
        emit textChanged(arg);
    }
}

void LabelItem::setFont(const QFont &arg)
{
    if (QGraphicsTextItem::font() != arg) {
        QGraphicsTextItem::setFont(arg);
        emit fontChanged(arg);
    }
}

QVariant LabelItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        if (isSelected()) {
            qDebug() << Q_FUNC_INFO << "set text cursor";
            setTextInteractionFlags(Qt::TextBrowserInteraction);
            setCursor(Qt::IBeamCursor);
        }
        else {
            qDebug() << Q_FUNC_INFO << "reset cursor";
            setTextInteractionFlags(Qt::NoTextInteraction);
            unsetCursor();
        }
    }
    return DrawingItem<QGraphicsTextItem>::itemChange(change, value);
}
