#ifndef GRAPHICSSELECTTOOL_H
#define GRAPHICSSELECTTOOL_H

#include "graphicstool.h"

class GraphicsSelectTool : public GraphicsTool
{
public:
    GraphicsSelectTool(QObject *parent = 0);
    ~GraphicsSelectTool();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    QDialog *optionDialog();

private:
    struct MouseData;
    MouseData *m_mouseData;

    // GraphicsTool interface
public slots:
    virtual void cancel();

    // GraphicsTool interface
public:
    virtual QString toolGroup() const;
    virtual QList<QAction *> actions() const;
};

#endif // GRAPHICSSELECTTOOL_H
