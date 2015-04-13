#ifndef COLORPROFILECOMBOBOX_H
#define COLORPROFILECOMBOBOX_H

#include <QComboBox>

class PcbPalette;

class ColorProfileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ColorProfileComboBox(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void addItem(PcbPalette *palette);
    void removeItem(PcbPalette *palette);
    void updateItem(PcbPalette *palette);
    void activatePalette(int index);
    void updateCurrentIndex(PcbPalette *palette);

private:
    int index(PcbPalette *paletteAt);
    PcbPalette *paletteAt(int index);
};

#endif // COLORPROFILECOMBOBOX_H
