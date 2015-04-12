#ifndef COLORPROFILEEDITOR_H
#define COLORPROFILEEDITOR_H

#include <QWidget>

class QListWidgetItem;
class QTreeWidgetItem;

namespace Ui {
class ColorProfileEditor;
}

class PcbPaletteManager;
class PcbPalette;

class ColorProfileEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColorProfileEditor(QWidget *parent = 0);
    ~ColorProfileEditor();

    void initialise();

protected:
    void cleanColorView();
    void populateColorView();

protected slots:
    void profileActivated(QListWidgetItem *item);
    void colorActivated(QTreeWidgetItem *item);

private:
    Ui::ColorProfileEditor *ui;
    PcbPaletteManager *m_manager;
    QListWidgetItem *m_activeProfileItem;
    PcbPalette *m_activeProfile;
};

#endif // COLORPROFILEEDITOR_H
