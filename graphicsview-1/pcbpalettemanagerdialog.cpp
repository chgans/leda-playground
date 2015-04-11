#include "pcbpalettemanagerdialog.h"
#include "pcbpalettemanager.h"

#include <QSplitter>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>


/*
 * TODO:
 *  - Better color dialog
 *  - Allow to define which profile is the user's default (prefereed?) one, exclusive checkboxes?
 *  - Allow to define which profile is the current one (for the current document), exclusive checkboxes?
 *  - Change layout, colors in a/several table(s), so that all colors can be seen without having to scroll
 *    and color editor as a dialog (click or double click to edit a color)
 *  - System profiles cannot be edited
 *  - use icon for system vs user
 *  - system folder/profiles (R/O), + user folder/profiles (R/W) + project folder/profile?
 *  - have an indocator for modified profiles (as in a document editor)
 */


typedef struct {
    const char *altium;
    PcbPalette::ColorRole leda;
} AltiumToLeda;

static const AltiumToLeda gAltiumToLeda[] = {
    { "TopLayer", PcbPalette::TopLayer },
    { "MidLayer1", PcbPalette::MidLayer1 },
    { "BottomLayer", PcbPalette::TopLayer }
};

// Format: LayerName=00BBGGRR
static const QColor fromAltium(const QString &str)
{
    QRegularExpression re("^00[0-9a-fA-f]{6}$");
    QRegularExpressionMatch rem = re.match(str);
    if (!re.isValid() || !rem.isValid() || !rem.hasMatch()) {
        return QColor();
    }
    bool ok;
    int r = str.mid(6, 2).toInt(&ok, 16);
    int g = str.mid(4, 2).toInt(&ok, 16);
    int b = str.mid(2, 2).toInt(&ok, 16);
    int a = 255;
    return QColor(r, g, b, a);
}

static QIcon makeIcon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

PcbPaletteManagerDialog::PcbPaletteManagerDialog(QWidget *parent) :
    QDialog(parent)
{
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    QWidget *leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftWidget->setLayout(leftLayout);
    splitter->addWidget(leftWidget);
    mPaletteList = new QListWidget;
    connect(mPaletteList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(activatePalette(QListWidgetItem*)));
    leftLayout->addWidget(new QLabel("Available Profiles"));
    leftLayout->addWidget(mPaletteList);
    leftLayout->addWidget(new QLabel("Profile location"));
    leftLayout->addWidget(new QLabel("..."));
    leftLayout->addWidget(new QPushButton("Explore folder"));
    leftLayout->addWidget(new QPushButton("Save profile..."));
    QPushButton *button = new QPushButton("Save profile as...");
    leftLayout->addWidget(button);
    connect(button, SIGNAL(clicked()),
            this, SLOT(saveCurrentProfileAs()));
    leftLayout->addWidget(new QPushButton("Load profile..."));
    leftLayout->addWidget(new QPushButton("Rename profile..."));
    leftLayout->addWidget(new QPushButton("Remove profile..."));
    button = new QPushButton("Import Altium\n2D Profile...");
    connect(button, SIGNAL(clicked()),
            this, SLOT(importAltiumProfile()));
    leftLayout->addWidget(button);

    QWidget *rightWidget = new QWidget;
    QHBoxLayout *rightLayout = new QHBoxLayout;
    mColorList = new QListWidget;
    connect(mColorList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(activateColor(QListWidgetItem*)));
    rightWidget->setLayout(rightLayout);
    splitter->addWidget(rightWidget);
    rightLayout->addWidget(mColorList);
    mColorDialog = new QColorDialog;
    mColorDialog->setOption(QColorDialog::NoButtons, true);
    mColorDialog->hide();
    connect(mColorDialog, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(setActiveColor(QColor)));
    rightLayout->addWidget(mColorDialog);



    setLayout(new QVBoxLayout);
    layout()->addWidget(splitter);
}

void PcbPaletteManagerDialog::initialise()
{
    PcbPaletteManager *mng = PcbPaletteManager::instance();
    QString current = mng->activePaletteIdentifier();
    mPaletteList->clear();
    mColorList->clear();
    foreach (QString id, mng->paletteIdentifiers()) {
        QListWidgetItem *item = new QListWidgetItem(id);
        mPaletteList->addItem(item);
        if (id == current) {
            item->setSelected(true);
            activatePalette(item);
        }
    }
}

void PcbPaletteManagerDialog::activatePalette(QListWidgetItem *item)
{
    PcbPaletteManager *mng = PcbPaletteManager::instance();
    QString id = item->text();
    PcbPalette *palette = mng->palette(id);
    mColorList->clear();
    foreach(PcbPalette::ColorRole role, palette->allValidColorRoles()) {
        QColor color = palette->color(role);
        QIcon icon = makeIcon(color);
        QString text = palette->colorRoleLabel(role);
        QListWidgetItem *item = new QListWidgetItem(icon, text);
        item->setData(Qt::UserRole, id);
        item->setData(Qt::UserRole+1, role);
        item->setData(Qt::UserRole+2, color);
        mColorList->addItem(item);
    }
}

void PcbPaletteManagerDialog::activateColor(QListWidgetItem *item)
{
    mColorDialog->setCurrentColor(item->data(Qt::UserRole+2).value<QColor>());
}

void PcbPaletteManagerDialog::setActiveColor(const QColor &color)
{
    QListWidgetItem *item = mColorList->currentItem();
    QString id = item->data(Qt::UserRole).value<QString>();
    PcbPalette::ColorRole role;
    role = static_cast<PcbPalette::ColorRole>(item->data(Qt::UserRole+1).toInt());
    PcbPaletteManager *mng = PcbPaletteManager::instance();
    PcbPalette *palette = mng->palette(id);
    palette->setColor(role, color);

    QIcon icon = makeIcon(color);
    item->setIcon(icon);
}

void PcbPaletteManagerDialog::saveCurrentProfileAs()
{
    QListWidgetItem *item = mPaletteList->currentItem();
    if (!item)
        return;

    QString id = item->text();
    PcbPaletteManager *mng = PcbPaletteManager::instance();
    PcbPalette *palette = mng->palette(id);

    QString fileSuffix = "LedaPcbPalette";
    QString fileFilter = QString("*.%1").arg(fileSuffix);
    QString filename = QFileDialog::getSaveFileName(this,
                                                "Save selected profile as",
                                                "",
                                                fileFilter);
    if (filename.isEmpty())
        return;

    QFileInfo fileInfo(filename);
    QString newId = fileInfo.baseName();
    if (fileInfo.completeSuffix() != fileSuffix)
        filename.append(QString(".%1").arg(fileSuffix));

    QSettings settings(filename, QSettings::IniFormat);
    palette->saveToSettings(settings);
    settings.sync();

    PcbPalette *newPalette = new PcbPalette;
    newPalette->loadFromSettings(settings);
    mng->addPalette(newId, newPalette);
    initialise(); // FIXME: monitor paletteAdded/Removed
}

void PcbPaletteManagerDialog::importAltiumProfile()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select one or more files to import",
                                                      "/home",
                                                      "Altium 2D Color Profile (*.PCBSysColors)");
    if (files.isEmpty())
        return;

    PcbPaletteManager *mng = PcbPaletteManager::instance();

    foreach (QString filename, files) {
        QFileInfo fileInfo(filename);
        QString id = fileInfo.baseName();
        // FIXME: do a lowercase compare + unix vs windoz vs mac
        if (mng->paletteIdentifiers().contains(id)) {
            QMessageBox::warning(this, "Altium import",
                                 QString("A color palette with the \"%1\" identifier "
                                         "already exists").arg(id));
            continue;
        }

        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Altium import",
                                 QString("Could not open %1 for reading, profile not "
                                         "imported!").arg(id));
            continue;
        }
        file.close();

        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        if (!settings.childGroups().contains("LayerColors") ||
            !settings.childGroups().contains("WorkspaceColors")) {
            QMessageBox::warning(this, "Altium import",
                                 QString("%1: file format not recognised, profile not "
                                         "imported!\nReason: Missing settings group(s)").arg(id));
            continue;
        }

        PcbPalette *palette = new PcbPalette;
        //settings.beginGroup("LayerColors");
        int errors = 0;
        foreach(PcbPalette::ColorRole role, palette->allValidColorRoles()) {
            QString name = palette->colorRoleToAltiumName(role);
            QString value = settings.value(name).toString();
            QColor color = fromAltium(value);
            if (!color.isValid())
                errors++;
            else
                palette->setColor(role, color);
        }
        if (errors > 16) {
            QMessageBox::warning(this, "Altium import",
                                 QString("%1: file format not recognised, profile not "
                                         "imported!\nReason: too many parsing errors").arg(id));
            continue;
        }
        else if (errors > 0) {
            QMessageBox::information(this, "Altium import",
                                     QString("%1: Profile imported with %2 "
                                             "error(s)").arg(id).arg(errors));
        }
        mng->addPalette(id, palette);
        initialise();
    }
}

