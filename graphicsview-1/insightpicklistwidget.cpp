#include "insightpicklistwidget.h"
#include "objectpreview.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QSplitter>
#include <QApplication>
#include <QPalette>

#include <QDebug>

// FIXME: Using the splitter screws the sizing
InsightPickListWidget::InsightPickListWidget(QWidget *parent) :
    QFrame(parent)
{
    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box);
    setMaximumHeight(300);

    // Create the view, we snoop on mouse move for moving ourself
    mView = new ObjectPreview;
    mView->setCursor(Qt::OpenHandCursor);
    mView->viewport()->installEventFilter(this);
    mView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mView->setMaximumHeight(150);

    // Create the item list view
    mTable = new QTableWidget;
    mTable->setColumnCount(4);
    mTable->horizontalHeader()->hide();
    mTable->verticalHeader()->hide();
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //mTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    mTable->setWordWrap(false);
    mTable->setTextElideMode(Qt::ElideNone);
    mTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    // Style it
    mTable->setFrameStyle(QFrame::NoFrame);
    mTable->setShowGrid(false);

    // update view when mouse hover items
    mTable->setMouseTracking(true);
    connect(mTable, SIGNAL(itemEntered(QTableWidgetItem*)),
            this, SLOT(activateTableItem(QTableWidgetItem*)));

    // emit select when mouse clicked
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(mTable, SIGNAL(clicked(QModelIndex)),
            this, SLOT(selectItem(QModelIndex)));

    // Setup layout
    setLayout(new QVBoxLayout());
    layout()->addWidget(mView);
    layout()->addWidget(mTable);
    layout()->setMargin(5);
}

InsightPickListWidget::~InsightPickListWidget()
{
    // FIXME: we never get destroyed!
    qDebug() << __PRETTY_FUNCTION__;
}

void InsightPickListWidget::setPickList(QGraphicsScene *scene, QList<QGraphicsItem *> list)
{
    mTable->clear();

    if (list.count() == 0)
        return;

    mView->setScene(scene);

    mTable->setRowCount(list.size());
    QTableWidgetItem *item;
    for (int i=0; i<list.size(); ++i) {
        item = new QTableWidgetItem(QIcon(":/img/chip-package.png"), "chip");
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        mTable->setItem(i, 0, item);
        item = new QTableWidgetItem(QString("Item %1").arg(i));
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        mTable->setItem(i, 1, item);
        item = new QTableWidgetItem(QString("Square(300, 200, 150, 150)"));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        mTable->setItem(i, 2, item);
        item = new QTableWidgetItem(QString("Top layer"));
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        mTable->setItem(i, 3, item);
    }
    activateTableItem(mTable->item(0, 0));
}

void InsightPickListWidget::activateTableItem(QTableWidgetItem *item)
{
    mTable->selectRow(item->row());
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    mView->setObjectToPreview(gitem);
}

void InsightPickListWidget::selectItem(const QModelIndex &index)
{
    QTableWidgetItem *item = mTable->item(index.row(), index.column());
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    emit itemSelected(gitem);
}

bool InsightPickListWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (mView && mView->viewport() && obj == mView->viewport()) {
        // Start move
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mev = static_cast<QMouseEvent *>(event);
            if (mev->button() == Qt::LeftButton) {
                mMousePos = mapToParent(mapFromGlobal(mev->globalPos()));
                mView->setCursor(Qt::ClosedHandCursor);
            }
        }
        // Do move
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mev = static_cast<QMouseEvent *>(event);
            if (mev->buttons() == Qt::LeftButton) {
                QPoint p = mapToParent(mapFromGlobal(mev->globalPos()));
                move(pos() + p - mMousePos);
                mMousePos = p;
            }
        }
        // Stop move
        else if (event->type() == QEvent::MouseButtonRelease) {
            mView->setCursor(Qt::OpenHandCursor);
        }
    }
    return QFrame::eventFilter(obj, event);
}
