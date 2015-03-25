#include "insightheadsupwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QColor>
#include <QGridLayout>
#include <QGraphicsView>

InsightHeadsUpWidget::InsightHeadsUpWidget(QWidget *parent) :
    QFrame(parent)
{
    // Mouse events go through to the buddy view
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    setFrameStyle(Box|Plain);
    setAutoFillBackground(true);
    setLineWidth(1);
    setStyleSheet("QFrame { background-color:rgba(0,0,0,128); }"
                  "QLabel { color: #ffffdd; background-color: none; font-weight: bold; }");
    // TODO: tweak font/size

    mCoordLayout = new QGridLayout;
    mX = new QLabel("X: 125.365");
    mX->setAutoFillBackground(false);
    mY = new QLabel("Y: 56.325");
    mY->setAutoFillBackground(false);
    mDX = new QLabel("dX: 0.000");
    mDX->setAutoFillBackground(false);
    mDY = new QLabel("dY: 0.000");
    mDY->setAutoFillBackground(false);
    mCoordLayout->addWidget(mX, 0, 0);
    mCoordLayout->addWidget(mY, 1, 0);
    mCoordLayout->addWidget(mDX, 0, 1);
    mCoordLayout->addWidget(mDY, 1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addLayout(mCoordLayout);
    layout->addStretch();

    mView = static_cast<QGraphicsView*>(parent);
    mView->installEventFilter(this);
}

// TODO: Let the view compute the position (using snap/grid) and notify observers
// either via signal or via IDrawingViewObserver registration + event mask
bool InsightHeadsUpWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == mView) {
        updateCoordinates();
    }
    return QWidget::eventFilter(watched, event);
}

void InsightHeadsUpWidget::updateCoordinates()
{
    QPoint p = mView->viewport()->mapFromGlobal(QCursor::pos());
    QPointF pf = mView->mapToScene(p);
    mX->setText(QString("X: %1").arg(pf.x(), 0, 'f', 3));
    mY->setText(QString("Y: %1").arg(pf.y(), 0, 'f', 3));
    mCoordLayout->update();
}
