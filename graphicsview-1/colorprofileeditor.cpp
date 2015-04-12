#include "colorprofileeditor.h"
#include "ui_colorprofileeditor.h"

#include "pcbpalette.h"
#include "pcbpalettemanager.h"
#include "designlayer.h"
#include "designlayermanager.h"

#include <QTreeWidgetItem>
#include <QTableWidgetItem>

#include <QDebug>

static QIcon icon(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

ColorProfileEditor::ColorProfileEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorProfileEditor),
    m_manager(nullptr),
    m_activeProfile(nullptr)
{
    ui->setupUi(this);

    ui->colorWidget->setAlphaEnabled(false);
    ui->colorWidget->setPreviewDisplayMode(ColorPreview::SplitColor);
    ui->colorWidget->setColor(Qt::white);
    ui->colorWidget->setEnabled(false);

    // If using FancyLineEdit
    //ui->profileFilter->setButtonVisible(Utils::FancyLineEdit::Left, true);
    //ui->colorFilter->setButtonVisible(Utils::FancyLineEdit::Left, true);

    ui->profileTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->profileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->profileTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->profileTable->horizontalHeader()->setStretchLastSection(true);
    ui->profileTable->horizontalHeader()->hide();
    ui->profileTable->verticalHeader()->hide();
    connect(ui->profileTable, &QTableWidget::itemSelectionChanged,
            this, &ColorProfileEditor::profileSelectionChanged);

    ui->colorTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->colorTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->colorTree, &QTreeWidget::itemSelectionChanged,
            this, &ColorProfileEditor::colorSelectionChanged);

}

ColorProfileEditor::~ColorProfileEditor()
{
    delete ui;
}

void ColorProfileEditor::initialise()
{
    m_manager = PcbPaletteManager::instance();
    ui->profileTable->clear();
    ui->profileTable->setRowCount(m_manager->palettes().count());
    ui->profileTable->setColumnCount(2);
    int i = 0;
    foreach (const PcbPalette *palette, m_manager->palettes()) {
        QString id = palette->name();
        QTableWidgetItem *nameItem = new QTableWidgetItem();
        nameItem->setText(id);
        nameItem->setData(Qt::UserRole, QVariant::fromValue<const PcbPalette*>(palette));
        if (!palette->isSystemPalette())
            nameItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
        else
            nameItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        QTableWidgetItem *attItem = new QTableWidgetItem();
        attItem->setText(palette->isSystemPalette() ? "System" : "User");
        attItem->setData(Qt::UserRole, QVariant::fromValue<const PcbPalette*>(palette));
        attItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        ui->profileTable->setItem(i, 0, nameItem);
        ui->profileTable->setItem(i, 1, attItem);
        i++;
    }
}

void ColorProfileEditor::cleanColorView()
{
    ui->colorTree->clear();
}

void ColorProfileEditor::populateColorView()
{
    DesignLayerManager *manager = DesignLayerManager::instance();


    for (DesignLayer::Category category = DesignLayer::SignalCategory;
         category <= DesignLayer::OtherCategory;
         category = DesignLayer::Category(category + 1)) {

        QTreeWidgetItem *groupItem = new QTreeWidgetItem();
        groupItem->setText(0, manager->categoryName(category));
        groupItem->setFlags(Qt::ItemIsEnabled);
        groupItem->setData(0, Qt::UserRole,
                           QVariant::fromValue<DesignLayer::Category>(category));

        foreach (DesignLayer *layer, manager->layersForCategory(category)) {
            QTreeWidgetItem *colorItem =  new QTreeWidgetItem();
            PcbPalette::ColorRole role = PcbPalette::ColorRole(layer->stackPosition() + 1);
            QColor color = m_activeProfile->color(role);
            colorItem->setText(0, layer->name());
            colorItem->setBackgroundColor(1, color);
            colorItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            colorItem->setData(0, Qt::DecorationRole,
                               icon(color));
            colorItem->setData(0, Qt::UserRole,
                               QVariant::fromValue<QColor>(color));
            groupItem->addChild(colorItem);
        }

        ui->colorTree->addTopLevelItem(groupItem);
    }
    ui->colorTree->expandAll();
    ui->colorTree->resizeColumnToContents(0);
}

void ColorProfileEditor::profileSelectionChanged()
{
    if (ui->profileTable->selectedItems().count() == 0)
        return;

    QTableWidgetItem *item = ui->profileTable->selectedItems().first();
    const PcbPalette *profile = item->data(Qt::UserRole).value<const PcbPalette *>();
    if (m_activeProfile == profile)
        return;
    m_activeProfile = profile;
    ui->colorWidget->setVisible(false);
    if (m_activeProfile) {
        ui->colorWidget->setEnabled(!m_activeProfile->isSystemPalette());
        cleanColorView();
        populateColorView();
        qDebug() << m_activeProfile->name();
    }
}

void ColorProfileEditor::colorSelectionChanged()
{
    if (ui->colorTree->selectedItems().count() == 0)
        return;

    QTreeWidgetItem *item = ui->colorTree->selectedItems().first();
    bool isColor = item->data(0, Qt::UserRole).canConvert<QColor>();
    ui->colorWidget->setVisible(isColor);
    qDebug() << item->text(0);
    if (isColor)
        ui->colorWidget->setColor(item->data(0, Qt::UserRole).value<QColor>());
}
