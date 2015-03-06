#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>


class GraphicsView;
class GraphicsScene;
class QGraphicsItem;

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;

class QDialog;
class QAction;

/* TBD:
 *  - Actions (hooked with context menu)
 *    - Esc => cancel()
 *    - Tab => options
 */

class GraphicsTool : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsTool(QObject *parent = 0);
    ~GraphicsTool();

    GraphicsView *view();
    GraphicsScene *scene();
    void setView(GraphicsView *view);

    // TODO: split interactive vs non-interactive
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

    virtual QDialog *optionDialog() = 0;

    virtual QString toolGroup() const = 0;
    virtual QAction *action() const = 0;

signals:
    void finished();

public slots:
    virtual void cancel();

protected:
    QGraphicsItem *createPhantomItem(QGraphicsItem *item);
    QList<QGraphicsItem *> createPhantomItems(QList<QGraphicsItem *> items);

private:
    GraphicsView *m_view;
};

Q_DECLARE_METATYPE(GraphicsTool*)

#endif // GRAPHICSTOOL_H
