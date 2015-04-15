#include "pcbpalettesettingsdialog.h"
#include "ui_pcbpalettesettingsdialog.h"
#include "pcbpalette.h"

/*
struct {
    PcbPalette::ColorRole role;
    const char *str;
} SignalsStruct;

static const SignalStructs SIGNALS[] = {
    { PcbPalette::SignalLayer1, "Top Layer"},
    { PcbPalette::SignalLayer2, "Mid-Layer"},
    { PcbPalette::SignalLayer3, "Top Layer"},
    { PcbPalette::SignalLayer4, "Top Layer"},
    { PcbPalette::SignalLayer5, "Top Layer"},
};
*/

static QIcon *icon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return new QIcon(pix);
}

PcbPaletteSettingsDialog::PcbPaletteSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PcbPaletteSettingsDialog)
{
    PcbPalette palette;
    QSettings s("pcb-palette.conf", QSettings::IniFormat);
    palette.loadFromSettings(s);
#if 0
    /* TODO: import of Altium theme.PCBSysColors files
       Altium use ABGR, Qt ARGB */
    for (int i=0; i<128; ++i) {
        PcbPalette::ColorRole role = static_cast<PcbPalette::ColorRole>(i);
        QColor color = palette.color(role);
        int r = color.red();
        color.setRed(color.blue());
        color.setBlue(r);
        palette.setColor(role, color);
    }
    palette.saveToSettings(s);
#endif

    ui->setupUi(this);
    ui->signalTable->verticalHeader()->hide();
    ui->signalTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->signalTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->signalTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->signalTable->setRowCount(32);
    QTableWidgetItem *label, *color, *show;
    label = new QTableWidgetItem("Top Layer");
    label->setFlags(Qt::ItemIsEnabled);
    ui->signalTable->setItem(0, 0, label);
    color = new QTableWidgetItem(*icon(palette.color(static_cast<PcbPalette::ColorRole>(0+1))), "");
    color->setFlags(Qt::ItemIsEnabled);
    ui->signalTable->setItem(0, 1, color);
    show = new QTableWidgetItem("");
    show->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    ui->signalTable->setItem(0, 2, show);

    for (int i=1; i<31; ++i) {
        label = new QTableWidgetItem(QString("Mid-Layer %1").arg(i));
        label->setFlags(Qt::NoItemFlags);
        ui->signalTable->setItem(i, 0, label);
        color = new QTableWidgetItem(*icon(palette.color(static_cast<PcbPalette::ColorRole>(i+1))), "");
        color->setFlags(Qt::ItemIsEnabled);
        ui->signalTable->setItem(i, 1, color);
        show = new QTableWidgetItem("");
        show->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
        ui->signalTable->setItem(i, 2, show);

    }
    label = new QTableWidgetItem("Top Layer");
    label->setFlags(Qt::ItemIsEnabled);
    ui->signalTable->setItem(31, 0, label);
    color = new QTableWidgetItem(*icon(palette.color(static_cast<PcbPalette::ColorRole>(31+1))), "");
    color->setFlags(Qt::ItemIsEnabled);
    ui->signalTable->setItem(31, 1, color);
    show = new QTableWidgetItem("");
    show->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    ui->signalTable->setItem(31, 2, show);

}

PcbPaletteSettingsDialog::~PcbPaletteSettingsDialog()
{
    delete ui;
}
