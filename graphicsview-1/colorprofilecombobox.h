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
    void addItem(const PcbPalette *palette);
    void removeItem(const PcbPalette *palette);
    void updateItem(const PcbPalette *palette);
    void activatePalette(int index);
    void updateCurrentIndex(const PcbPalette *palette);

private:
    int index(const PcbPalette *paletteAt);
    const PcbPalette *paletteAt(int index);
};

#endif // COLORPROFILECOMBOBOX_H
