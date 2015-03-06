#ifndef GRAPHICSLINETOOLDIALOG_H
#define GRAPHICSLINETOOLDIALOG_H

#include <QDialog>

namespace Ui {
class GraphicsLineToolDialog;
}

class GraphicsLineToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphicsLineToolDialog(QWidget *parent = 0);
    ~GraphicsLineToolDialog();

private:
    Ui::GraphicsLineToolDialog *ui;
};

#endif // GRAPHICSLINETOOLDIALOG_H
