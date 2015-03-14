#ifndef POLARGRIDEDITORWIDGET_H
#define POLARGRIDEDITORWIDGET_H

#include <igrideditor.h>

class QColorDialog;

namespace Ui {
class PolarGridEditorWidget;
}

class PolarGridEditor : public IGridEditor
{
    Q_OBJECT

public:
    explicit PolarGridEditor(QWidget *parent = 0);
    ~PolarGridEditor();

    virtual QString title() const;

    void loadFromGrid(const IGrid *grid);
    void saveToGrid(IGrid *grid) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::PolarGridEditorWidget *ui;
    QColorDialog *mColorDialog;
};

#endif // POLARGRIDEDITORWIDGET_H
