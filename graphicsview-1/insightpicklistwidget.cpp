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

// TODO:
//  * Add a splitter between the view and the list
//  * Get the list an automatic vertical size (depends on list length)
//  * Replace the list view with a "flat" table: type's icon, type's label, description, layer
//  * `-> the width of the QFrame is influenced by the description width
InsightPickListWidget::InsightPickListWidget(QWidget *parent) :
    QFrame(parent)
{
    //setWindowFlags(Qt::SplashScreen); // Is it right?
    setLayout(new QVBoxLayout);
    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box);

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    layout()->addWidget(splitter);

    // Create the view, w/ fixed size
    mView = new ObjectPreview;
    //mView->setMaximumSize(300, 150); // FIXME
    splitter->addWidget(mView);
    // Snoop on mouse move (for moving our self)
    mView->setCursor(Qt::OpenHandCursor);
    mView->viewport()->installEventFilter(this);

    // Create the item list view
    mList = new QListWidget;
    mList->viewport()->setAutoFillBackground(false);
    mList->setFrameStyle(QFrame::NoFrame);
    // update view when mouse hover items
    mList->setMouseTracking(true);
    connect(mList, SIGNAL(itemEntered(QListWidgetItem*)),
            this, SLOT(activateListItem(QListWidgetItem*)));
    // emit select when mouse clicked
    mList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(mList, SIGNAL(clicked(QModelIndex)),
            this, SLOT(selectItem(QModelIndex)));

    // Create the item list view
    mTable = new QTableWidget;
    mTable->setColumnCount(4);
    mTable->horizontalHeader()->hide();
    mTable->verticalHeader()->hide();
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    mTable->setWordWrap(false);
    mTable->setTextElideMode(Qt::ElideNone);
    mTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Style it
    //mTable->viewport()->setAutoFillBackground(false);
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

    //splitter->addWidget(mList);
    splitter->addWidget(mTable);

    // More compact layout
    layout()->setMargin(5);

    // Drive the size ourself
    setSizePolicy(QSizePolicy::Preferred,
                  QSizePolicy::Fixed);

    // For debugging
    //splitter->setFrameStyle(QFrame::Box);
    //splitter->setLineWidth(1);
}

InsightPickListWidget::~InsightPickListWidget()
{
    // FIXME: we never get destroyed!
    qDebug() << __PRETTY_FUNCTION__;
}

void InsightPickListWidget::setPickList(QGraphicsScene *scene, QList<QGraphicsItem *> list)
{
    mList->clear();
    mTable->clear();

    if (list.count() == 0)
        return;

    mView->setScene(scene);

    {
    QListWidgetItem *item;
    for (int i=0; i<list.size(); ++i) {
        item = new QListWidgetItem(QString("Item %1").arg(i), mList);
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
    }
    activateListItem(mList->item(0));
    }

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
    updateGeometry();
}

QSize InsightPickListWidget::sizeHint() const
{
    // FIXME!!!
    return QSize(mTable->sizeHint().width() + layout()->margin()*4 + 25,
                 150 + mTable->rowHeight(0)*mTable->rowCount()+15);
}

void InsightPickListWidget::activateListItem(QListWidgetItem *item)
{
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    mView->setObjectToPreview(gitem);
}

void InsightPickListWidget::activateTableItem(QTableWidgetItem *item)
{
    for (int r=0; r<mTable->rowCount(); ++r)
        for (int c=0; c<mTable->columnCount(); ++c)
            mTable->item(r, c)->setSelected(r==item->row());
    mTable->setRangeSelected(QTableWidgetSelectionRange(item->row(), 0, item->row(), 4), true);
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    mView->setObjectToPreview(gitem);
}

void InsightPickListWidget::selectItem(const QModelIndex &index)
{
    QListWidgetItem *item = mList->item(index.row());
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
