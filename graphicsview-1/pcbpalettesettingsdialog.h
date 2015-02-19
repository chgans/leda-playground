#ifndef PCBPALETTESETTINGSDIALOG_H
#define PCBPALETTESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class PcbPaletteSettingsDialog;
}

class PcbPaletteSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PcbPaletteSettingsDialog(QWidget *parent = 0);
    ~PcbPaletteSettingsDialog();

private:
    Ui::PcbPaletteSettingsDialog *ui;
};

#endif // PCBPALETTESETTINGSDIALOG_H
