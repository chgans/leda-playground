#ifndef PCBPALETTEMANAGERDIALOG_H
#define PCBPALETTEMANAGERDIALOG_H

#include <QDialog>

class PcbPalette;
class QListWidget;
class QListWidgetItem;
class QColorDialog;

class PcbPaletteManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PcbPaletteManagerDialog(QWidget *parent = 0);

    void initialise();

signals:

public slots:

protected slots:
    void activatePalette(QListWidgetItem *item);
    void activateColor(QListWidgetItem *item);
    void setActiveColor(const QColor & color);
    void saveCurrentProfileAs();
    void importAltiumProfile();

private:
    QListWidget *m_paletteList;
    QListWidget *m_colorList;
    QColorDialog *m_colorDialog;
};

#endif // PCBPALETTEMANAGERDIALOG_H
