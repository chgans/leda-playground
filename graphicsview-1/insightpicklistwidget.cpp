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
    m_view = new ObjectPreview;
    m_view->setCursor(Qt::OpenHandCursor);
    m_view->viewport()->installEventFilter(this);
    m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_view->setMaximumHeight(150);

    // Create the item list view
    m_table = new QTableWidget;
    m_table->setColumnCount(4);
    m_table->horizontalHeader()->hide();
    m_table->verticalHeader()->hide();
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_table->setWordWrap(false);
    m_table->setTextElideMode(Qt::ElideNone);
    m_table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    // Style it
    m_table->setFrameStyle(QFrame::NoFrame);
    m_table->setShowGrid(false);

    // update view when mouse hover items
    m_table->setMouseTracking(true);
    connect(m_table, &QTableWidget::itemEntered,
            this, &InsightPickListWidget::activateTableItem);

    // emit select when mouse clicked
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_table, &QAbstractItemView::clicked,
            this, &InsightPickListWidget::selectItem);

    // Setup layout
    setLayout(new QVBoxLayout());
    layout()->addWidget(m_view);
    layout()->addWidget(m_table);
    layout()->setMargin(5);
}

InsightPickListWidget::~InsightPickListWidget()
{
    // FIXME: we never get destroyed!
    qDebug() << __PRETTY_FUNCTION__;
}

void InsightPickListWidget::setPickList(QGraphicsScene *scene, QList<QGraphicsItem *> list)
{
    m_table->clear();

    if (list.count() == 0)
        return;

    m_view->setScene(scene);

    m_table->setRowCount(list.size());
    QTableWidgetItem *item;
    for (int i=0; i<list.size(); ++i) {
        item = new QTableWidgetItem(QIcon(":/img/chip-package.png"), "chip");
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_table->setItem(i, 0, item);
        item = new QTableWidgetItem(QString("Item %1").arg(i));
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_table->setItem(i, 1, item);
        item = new QTableWidgetItem(QString("Square(300, 200, 150, 150)"));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        m_table->setItem(i, 2, item);
        item = new QTableWidgetItem(QString("Top layer"));
        item->setData(Qt::UserRole, QVariant::fromValue(list.at(i)));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_table->setItem(i, 3, item);
    }
    activateTableItem(m_table->item(0, 0));
}

void InsightPickListWidget::activateTableItem(QTableWidgetItem *item)
{
    m_table->selectRow(item->row());
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    m_view->setObjectToPreview(gitem);
}

void InsightPickListWidget::selectItem(const QModelIndex &index)
{
    QTableWidgetItem *item = m_table->item(index.row(), index.column());
    QGraphicsItem *gitem = item->data(Qt::UserRole).value<QGraphicsItem *>();
    emit itemSelected(gitem);
}

bool InsightPickListWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (m_view && m_view->viewport() && obj == m_view->viewport()) {
        // Start move
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mev = static_cast<QMouseEvent *>(event);
            if (mev->button() == Qt::LeftButton) {
                m_mousePos = mapToParent(mapFromGlobal(mev->globalPos()));
                m_view->setCursor(Qt::ClosedHandCursor);
            }
        }
        // Do move
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mev = static_cast<QMouseEvent *>(event);
            if (mev->buttons() == Qt::LeftButton) {
                QPoint p = mapToParent(mapFromGlobal(mev->globalPos()));
                move(pos() + p - m_mousePos);
                m_mousePos = p;
            }
        }
        // Stop move
        else if (event->type() == QEvent::MouseButtonRelease) {
            m_view->setCursor(Qt::OpenHandCursor);
        }
    }
    return QFrame::eventFilter(obj, event);
}
