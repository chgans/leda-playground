#ifndef COLORPROFILETOOLBUTTON_H
#define COLORPROFILETOOLBUTTON_H

#include <QToolButton>

class QMenu;
class QActionGroup;

class PcbPalette;

class ColorProfileToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorProfileToolButton(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void addPalette(PcbPalette *palette);
    void removePalette(PcbPalette *palette);
    void updatePalette(PcbPalette *palette);

private:
    QMenu *m_menu;
    QActionGroup *m_actionGroup;
    QAction *action(PcbPalette *palette);
    PcbPalette *palette(QAction *action);
};

#endif // COLORPROFILETOOLBUTTON_H
