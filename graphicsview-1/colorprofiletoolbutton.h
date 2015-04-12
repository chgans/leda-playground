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
    void addPalette(const PcbPalette *palette);
    void removePalette(const PcbPalette *palette);
    void updatePalette(const PcbPalette *palette);

private:
    QMenu *m_menu;
    QActionGroup *m_actionGroup;
    QAction *action(const PcbPalette *palette);
    const PcbPalette *palette(QAction *action);
};

#endif // COLORPROFILETOOLBUTTON_H
