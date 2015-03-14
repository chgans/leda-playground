#include "cartesiangrideditor.h"
#include "ui_cartesiangrideditorwidget.h"
#include "cartesiangrid.h"

#include <QColorDialog>
#include <QDebug>

static int lineStyleToIndex(Qt::PenStyle style)
{
    switch (style) {
    case Qt::SolidLine: return 0;
    case Qt::DotLine: return 1;
    case Qt::NoPen: return 2;
    default:
        return 0;
    }
}

static Qt::PenStyle indexToLineStyle(int index)
{
    switch (index) {
    case 0: return Qt::SolidLine;
    case 1: return Qt::DotLine;
    case 2: return Qt::NoPen;
    default: return Qt::SolidLine;
    }
}

static int indexToMultiplier(int index)
{
    switch (index) {
    case 0: return 2;
    case 1: return 5;
    case 2: return 10;
    default: return 2;
    }
}

static int multiplierToIndex(int multiplier)
{
    if (multiplier <= 2)
        return 0;
    else if (multiplier <= 5)
        return 1;
    else
        return 2;
}

CartesianGridEditor::CartesianGridEditor(QWidget *parent) :
    IGridEditor(parent),
    ui(new Ui::CartesianGridEditorWidget),
    mColorDialog(new QColorDialog)
{
    ui->setupUi(this);
    ui->fineLineColor->installEventFilter(this);
    ui->coarseLineColor->installEventFilter(this);
}

CartesianGridEditor::~CartesianGridEditor()
{
    delete ui;
}

QString CartesianGridEditor::title() const
{
    return QStringLiteral("Cartesian Grid Editor");
}

void CartesianGridEditor::loadFromGrid(const IGrid *grid)
{
    const CartesianGrid *cgrid = static_cast<const CartesianGrid*>(grid);

    ui->name->setText(cgrid->label());
    ui->unit->setCurrentIndex(0);
    ui->unit->setEnabled(false);
    ui->fineLineStyle->setCurrentIndex(lineStyleToIndex(cgrid->fineLineStyle()));
    ui->fineLineColor->setPalette(cgrid->fineLineColor());
    ui->fineLineColor->setAutoFillBackground(true);
    ui->coarseLineStyle->setCurrentIndex(lineStyleToIndex(cgrid->coarseLineStyle()));
    ui->coarseLineColor->setPalette(cgrid->coarseLineColor());
    ui->coarseLineColor->setAutoFillBackground(true);
    ui->coarseMultiplier->setCurrentIndex(multiplierToIndex(cgrid->coarseMultiplier()));
    ui->originX->setValue(cgrid->origin().x());
    ui->originY->setValue(cgrid->origin().y());

    ui->rotation->setValue(cgrid->rotation());
    ui->stepX->setValue(cgrid->step().x());
    ui->stepY->setValue(cgrid->step().y());
    ui->extentX->setValue(cgrid->size().width());
    ui->extentY->setValue(cgrid->size().height());
    ui->quadrantBottomLeft->setChecked(cgrid->isQuadrantEnabled(CartesianGrid::BottomLeftQuadrant));
    ui->quadrantBottomRight->setChecked(cgrid->isQuadrantEnabled(CartesianGrid::BottomRightQuadrant));
    ui->quadrantTopLeft->setChecked(cgrid->isQuadrantEnabled(CartesianGrid::TopLeftQuadrant));
    ui->quadrantTopRight->setChecked(cgrid->isQuadrantEnabled(CartesianGrid::TopRightQuadrant));

    // Priority
    // Enbled for comp and non-comp
}

void CartesianGridEditor::saveToGrid(IGrid *grid) const
{
    CartesianGrid *cgrid = static_cast<CartesianGrid*>(grid);

    cgrid->setLabel(ui->name->text());
    cgrid->setFineLineStyle(indexToLineStyle(ui->fineLineStyle->currentIndex()));
    cgrid->setFineLineColor(ui->fineLineColor->palette().color(QPalette::Button));
    cgrid->setCoarseLineStyle(indexToLineStyle(ui->coarseLineStyle->currentIndex()));
    cgrid->setCoarseMultiplier(indexToMultiplier(ui->coarseMultiplier->currentIndex()));
    cgrid->setCoarseLineColor(ui->coarseLineColor->palette().color(QPalette::Button));
    cgrid->setOrigin(QPointF(ui->originX->value(), ui->originY->value()));

    cgrid->setRotation(ui->rotation->value());
    cgrid->setSize(QSizeF(ui->extentX->value(), ui->extentY->value()));
    cgrid->setStep(QPointF(ui->stepX->value(), ui->stepY->value()));
    cgrid->setQuadrantEnabled(CartesianGrid::BottomLeftQuadrant, ui->quadrantBottomLeft->isChecked());
    cgrid->setQuadrantEnabled(CartesianGrid::BottomRightQuadrant, ui->quadrantBottomRight->isChecked());
    cgrid->setQuadrantEnabled(CartesianGrid::TopLeftQuadrant, ui->quadrantTopLeft->isChecked());
    cgrid->setQuadrantEnabled(CartesianGrid::TopRightQuadrant, ui->quadrantTopRight->isChecked());

    //grid->set
}

bool CartesianGridEditor::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::MouseButtonPress)
        return false;
    QString kind;
    if (watched == ui->fineLineColor)
        kind = "Fine";
    else if (watched == ui->coarseLineColor)
        kind = "Coarse";
    else
        return false;

    QWidget *who = static_cast<QWidget*>(watched);
    const QColor color = QColorDialog::getColor(who->palette().color(QPalette::Button),
                                                this, QString("Select %1 Color").arg(kind));
    if (color.isValid()) {
        who->setPalette(QPalette(color));
    }

    return false;
}
