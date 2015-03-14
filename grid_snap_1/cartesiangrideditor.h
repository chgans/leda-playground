#ifndef CARTESIANGRIDEDITOR_H
#define CARTESIANGRIDEDITOR_H

#include <igrideditor.h>

class QColorDialog;

namespace Ui {
class CartesianGridEditorWidget;
}

class CartesianGridEditor : public IGridEditor
{
    Q_OBJECT

public:
    explicit CartesianGridEditor(QWidget *parent = 0);
    ~CartesianGridEditor();

    virtual QString title() const;

    void loadFromGrid(const IGrid *grid);
    void saveToGrid(IGrid *grid) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::CartesianGridEditorWidget *ui;
    QColorDialog *mColorDialog;
};

#endif // CARTESIANGRIDEDITORWIDGET_H
