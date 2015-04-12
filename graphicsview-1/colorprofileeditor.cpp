#include "colorprofileeditor.h"
#include "ui_colorprofileeditor.h"

#include "pcbpalette.h"
#include "pcbpalettemanager.h"
#include "designlayer.h"
#include "designlayermanager.h"

#include <QTreeWidgetItem>
#include <QListWidgetItem>

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
    m_activeProfileItem(nullptr),
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

    ui->profileList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->profileList->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->profileList, &QListWidget::itemActivated,
            this, &ColorProfileEditor::profileActivated);

    ui->colorTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->colorTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->colorTree, &QTreeWidget::itemActivated,
            this, &ColorProfileEditor::colorActivated);
}

ColorProfileEditor::~ColorProfileEditor()
{
    delete ui;
}

void ColorProfileEditor::initialise()
{
    m_manager = PcbPaletteManager::instance();
    ui->profileList->addItems(m_manager->paletteIdentifiers());
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

void ColorProfileEditor::profileActivated(QListWidgetItem *item)
{
    qDebug() << item->text();
    if (m_activeProfileItem == item)
        return;
    if (m_activeProfileItem)
        cleanColorView();
    m_activeProfileItem = item;
    m_activeProfile = m_manager->palette(m_activeProfileItem->text());
    populateColorView();
}

void ColorProfileEditor::colorActivated(QTreeWidgetItem *item)
{
    bool isColor = item->data(0, Qt::UserRole).canConvert<QColor>();
    ui->colorWidget->setEnabled(isColor);
    if (isColor)
        ui->colorWidget->setColor(item->data(0, Qt::UserRole).value<QColor>());
    else
        ui->colorWidget->setColor(Qt::white);
}
