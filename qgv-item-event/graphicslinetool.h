#ifndef GRAPHICSLINETOOL_H
#define GRAPHICSLINETOOL_H

#include "graphicstool.h"

class GraphicsLineItem;

class GraphicsLineTool : public GraphicsTool
{
public:
    GraphicsLineTool(QObject *parent = 0);

    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;

public slots:
    virtual void cancel();

protected:
    void setP1(const QPoint &point);
    void setP2(const QPoint &point);
private:
    int m_state;
    GraphicsLineItem *m_line;
};

#endif // GRAPHICSLINETOOL_H
