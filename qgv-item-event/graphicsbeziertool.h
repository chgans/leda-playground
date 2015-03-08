#ifndef GRAPHICSBEZIERTOOL_H
#define GRAPHICSBEZIERTOOL_H

#include "graphicstool.h"

class GraphicsBezierItem;

class GraphicsBezierTool : public GraphicsTool
{
public:
    GraphicsBezierTool(QObject *parent);

private:
    GraphicsBezierItem *m_item;
    int m_elementNb;

    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    virtual void activate();
    virtual void desactivate();
};

#endif // GRAPHICSBEZIERTOOL_H
