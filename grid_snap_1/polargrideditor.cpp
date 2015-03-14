#include "polargrideditor.h"
#include "ui_polargrideditorwidget.h"
#include "polargrid.h"

#include <QColorDialog>

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

PolarGridEditor::PolarGridEditor(QWidget *parent) :
    IGridEditor(parent),
    ui(new Ui::PolarGridEditorWidget),
    mColorDialog(new QColorDialog)
{
    ui->setupUi(this);
    ui->fineLineColor->installEventFilter(this);
    ui->coarseLineColor->installEventFilter(this);
}

PolarGridEditor::~PolarGridEditor()
{
    delete ui;
}

QString PolarGridEditor::title() const
{
    return QStringLiteral("Polar Grid Editor");
}

void PolarGridEditor::loadFromGrid(const IGrid *grid)
{
    const PolarGrid *pgrid = static_cast<const PolarGrid*>(grid);

    ui->name->setText(pgrid->label());
    ui->unit->setCurrentIndex(0);
    ui->unit->setEnabled(false);
    ui->fineLineStyle->setCurrentIndex(lineStyleToIndex(pgrid->fineLineStyle()));
    ui->fineLineColor->setPalette(pgrid->fineLineColor());
    ui->fineLineColor->setAutoFillBackground(true);
    ui->coarseLineStyle->setCurrentIndex(lineStyleToIndex(pgrid->coarseLineStyle()));
    ui->coarseLineColor->setPalette(pgrid->coarseLineColor());
    ui->coarseLineColor->setAutoFillBackground(true);
    ui->coarseMultiplier->setCurrentIndex(multiplierToIndex(pgrid->coarseMultiplier()));
    ui->originX->setValue(pgrid->origin().x());
    ui->originY->setValue(pgrid->origin().y());

    ui->stepAngle->setValue(pgrid->angularStep());
    ui->angleStart->setValue(pgrid->angularRange().first);
    ui->angleEnd->setValue(pgrid->angularRange().second);
    ui->stepRadius->setValue(pgrid->radialStep());
    ui->radiusStart->setValue(pgrid->radialRange().first);
    ui->radiusEnd->setValue(pgrid->radialRange().second);
}

void PolarGridEditor::saveToGrid(IGrid *grid) const
{
    PolarGrid *pgrid = static_cast<PolarGrid*>(grid);

    pgrid->setLabel(ui->name->text());
    pgrid->setFineLineStyle(indexToLineStyle(ui->fineLineStyle->currentIndex()));
    pgrid->setFineLineColor(ui->fineLineColor->palette().color(QPalette::Button));
    pgrid->setCoarseLineStyle(indexToLineStyle(ui->coarseLineStyle->currentIndex()));
    pgrid->setCoarseLineColor(ui->coarseLineColor->palette().color(QPalette::Button));
    pgrid->setCoarseMultiplier(indexToMultiplier(ui->coarseMultiplier->currentIndex()));
    pgrid->setOrigin(QPointF(ui->originX->value(), ui->originY->value()));

    pgrid->setAngularStep(ui->stepAngle->value());
    pgrid->setAngularRange(ui->angleStart->value(), ui->angleEnd->value());
    pgrid->setRadialStep(ui->stepRadius->value());
    pgrid->setRadialRange(ui->radiusStart->value(), ui->radiusEnd->value());
}

bool PolarGridEditor::eventFilter(QObject *watched, QEvent *event)
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

