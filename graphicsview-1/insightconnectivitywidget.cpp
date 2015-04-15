#include "insightconnectivitywidget.h"

#include "overview.h"

#include <QVBoxLayout>
#include <QTreeWidget>

InsightConnectivityWidget::InsightConnectivityWidget(QWidget *parent) :
    QFrame(parent)
{
    setWindowFlags(Qt::SplashScreen);
    setLayout(new QVBoxLayout);
    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box);
    //setMaximumSize(500, 500);
    m_view = new OverView;
    layout()->addWidget(m_view);
    QTreeWidget *tree = new QTreeWidget;
    tree->setColumnCount(1);
    for (int i = 0; i < 1; ++i) {
        QTreeWidgetItem *itemi = new QTreeWidgetItem(tree, QStringList(QString("Schematics %1").arg(i)));
        itemi->setIcon(0, QIcon(":/img/PCB-icon.png"));
        for (int j = 0; j < 2; ++j) {
            QTreeWidgetItem *itemj = new QTreeWidgetItem(itemi, QStringList(QString("Scematics %1.%2").arg(i).arg(j)));
            itemj->setIcon(0, QIcon(":/img/chip-package.png"));
        }
    }
    //tree->insertTopLevelItems(0, items);
    tree->setAutoFillBackground(false);
    tree->setHeaderHidden(true);
    tree->expandAll();
    tree->setItemsExpandable(false);
    tree->setRootIsDecorated(false);
    tree->setFrameStyle(QFrame::NoFrame);
    // TODO: rempve background, scroll bars and make sure all items are visible
    layout()->addWidget(tree);
}

void InsightConnectivityWidget::setBuddyView(QGraphicsView *view)
{
    m_view->setScene(view->scene());
}
