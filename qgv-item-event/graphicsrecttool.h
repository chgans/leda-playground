#ifndef GRAPHICSRECTTOOL_H
#define GRAPHICSRECTTOOL_H

#include "graphicstool.h"

class GraphicsRectItem;

class GraphicsRectTool : public GraphicsTool
{
    Q_OBJECT

public:
    GraphicsRectTool(QObject *parent);


    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;

    // GraphicsTool interface
public slots:
    virtual void cancel();

protected:
    void setP1(const QPoint &point);
    void setP2(const QPoint &point);

private:
    int m_state;
    GraphicsRectItem *m_item;
};

#endif // GRAPHICSRECTTOOL_H
