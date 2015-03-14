#ifndef IGRIDEDITOR_H
#define IGRIDEDITOR_H

#include <QWidget>

class IGrid;

class IGridEditor: public QWidget
{
    Q_OBJECT

public:
    IGridEditor(QWidget *parent = 0);
    virtual ~IGridEditor() {}

    virtual QString title() const = 0;

    virtual void loadFromGrid(const IGrid *grid) = 0;
    virtual void saveToGrid(IGrid *grid) const = 0;

};

#endif // IGRIDEDITOR_H
