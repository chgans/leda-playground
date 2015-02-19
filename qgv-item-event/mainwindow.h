#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsRectItem>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QKeyEvent>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class NoteItem: public QGraphicsTextItem
{
public:
    NoteItem(QGraphicsItem *parent = 0):
        QGraphicsTextItem(parent)
    {}

    NoteItem(const QString &text, QGraphicsItem *parent = 0):
        QGraphicsTextItem(text, parent)
    {
        //setTextWidth(100);
    }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        painter->save();
        painter->setPen(QPen(Qt::darkGreen, 0));
        painter->setBrush(Qt::yellow);
        painter->drawRect(boundingRect());
        painter->restore();
        QGraphicsTextItem::paint(painter, option, widget);
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }

    QRectF boundingRect() const
    {
        return QGraphicsTextItem::boundingRect().adjusted(-10, -10, 10, 10);
    }

};

class TextItem: public QGraphicsTextItem
{
public:
    TextItem(QGraphicsItem *parent = 0):
        QGraphicsTextItem(parent)
    {}

    TextItem(const QString &text, QGraphicsItem *parent = 0):
        QGraphicsTextItem(text, parent)
    {}

    void mousePressEvent(QGraphicsSceneMouseEvent * event)
    {
        qDebug() << __PRETTY_FUNCTION__ << textInteractionFlags();
        if (isSelected() && !(textInteractionFlags() & Qt::TextEditorInteraction)) {
            setTextInteractionFlags(Qt::TextEditorInteraction);
        }
        qDebug() << __PRETTY_FUNCTION__ << textInteractionFlags();
        QGraphicsTextItem::mousePressEvent(event);
    }

    void focusInEvent(QFocusEvent *event)
    {
        qDebug() << __PRETTY_FUNCTION__;
        QGraphicsTextItem::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent *event)
    {
        qDebug() << __PRETTY_FUNCTION__;
        //if (textInteractionFlags() & Qt::TextEditorInteraction) {
          setPlainText(toPlainText()); // unselect text if some was selected
          setTextInteractionFlags(Qt::NoTextInteraction);
        //}
        QGraphicsTextItem::focusOutEvent(event);
    }

    void keyPressEvent(QKeyEvent *event)
    {
        qDebug() << __PRETTY_FUNCTION__;
        if (event->key() == Qt::Key_Enter ||
                event->key() == Qt::Key_Return) {
            setPlainText(toPlainText());
            setTextInteractionFlags(Qt::NoTextInteraction);
            event->ignore();
        }
        else
            QGraphicsTextItem::keyPressEvent(event);
    }

};

class RectItem: public QGraphicsRectItem
{
public:
    RectItem(QGraphicsItem *parent = 0):
        QGraphicsRectItem(parent)
    {
        init();
    }

    RectItem(const QRectF &rect, QGraphicsItem *parent = 0):
        QGraphicsRectItem(rect, parent)
    {
        init();
    }

    RectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0):
        QGraphicsRectItem(x, y, width, height, parent)
    {
        init();
    }

    ~RectItem() {}

protected:
    QGraphicsDropShadowEffect *effect;

    void init()
    {
        effect = 0;
        TextItem *litem;
        qreal h;
        litem = new TextItem("U21", this);
        h = -litem->boundingRect().height();
        litem->setPos(0, h);
        litem->setFlag(QGraphicsItem::ItemIsSelectable, true);
        litem->setFlag(QGraphicsItem::ItemIsMovable, true);
        litem = new TextItem("NE555", this);
        litem->setPos(0, h - litem->boundingRect().height());
        litem->setFlag(QGraphicsItem::ItemIsSelectable, true);
        litem->setFlag(QGraphicsItem::ItemIsMovable, true);

        effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(4);
        effect->setEnabled(false);
        setGraphicsEffect(effect);
        setFlag(ItemSendsGeometryChanges, true);
    }

    QVariant itemChange(GraphicsItemChange change, const QVariant & value)
    {
        if (effect && (change & ItemSelectedChange)) {
            effect->setEnabled(isSelected());
            //qDebug() << __FILE__ << __LINE__ << pos() << scenePos();
        }
        return QGraphicsRectItem::itemChange(change, value);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawRect(rect());
        if (isSelected()) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::darkYellow);
            painter->drawEllipse(rect().topLeft(), 3, 3);
            painter->drawEllipse(rect().topRight(), 3, 3);
            painter->drawEllipse(rect().bottomLeft(), 3, 3);
            painter->drawEllipse(rect().bottomRight(), 3, 3);
        }
        if (0)
            QGraphicsRectItem::paint(painter, option, widget);
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addRect(rect());
        path.addEllipse(rect().topLeft(), 3, 3);
        path.addEllipse(rect().topRight(), 3, 3);
        path.addEllipse(rect().bottomLeft(), 3, 3);
        path.addEllipse(rect().bottomRight(), 3, 3);
        return path;
    }

    QRectF boundingRect() const
    {
        return shape().boundingRect() | childrenBoundingRect();
    }
};

class EditTool: public QObject
{
    Q_OBJECT

public:
    explicit EditTool(QObject *parent = 0):
        QObject(parent)
    {}

    ~EditTool()
    {}



};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Q_SLOT void init();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsRectItem *item;
};

#endif // MAINWINDOW_H
