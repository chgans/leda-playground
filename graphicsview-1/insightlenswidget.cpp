#include "insightlenswidget.h"

#include <QGraphicsView>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPaintEngine>

#include <QDebug>

InsightLensWidget::InsightLensWidget(QWidget *parent) :
    QFrame(parent),
    mView(new QGraphicsView),
    mBuddyView(0)
{
    // Mouse events go through to the buddy view
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // Set up our own view
    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    mView->setInteractive(false);

    // Setup the layout
    setLayout(new QVBoxLayout);
    layout()->setMargin(0);
    layout()->addWidget(mView);

    // Load default
    setFrameStyle(Panel|Plain);
    setLineWidth(5);
    setLensEnabled(false);
    setLensSize(QSize(300, 300));
    setLensShape(SquareLens);
    setMouseTracking(true);
    setLensZoomLevel(400);
}

void InsightLensWidget::setLensShape(LensShape shape)
{
    mShape = shape;

    // Apply a mask in Round mode
    fixupPaintingArtifacts();
    if (mShape == RoundLens) {
        setMask(QRegion(rect(), QRegion::Ellipse));
        // The view needs a smaller one to allow for border drawing
        int w = lineWidth();
        QRect r = rect().adjusted(w, w, -w, -w).translated(-w, -w);
        mView->setMask(QRegion(r, QRegion::Ellipse));
    }
    else {
        mView->clearMask();
        clearMask();
    }
}

void InsightLensWidget::toggleLensShape()
{
    if (mShape == SquareLens)
        setLensShape(RoundLens);
    else
        setLensShape(SquareLens);
}

InsightLensWidget::LensShape InsightLensWidget::lensShape() const
{
    return mShape;
}

void InsightLensWidget::setLensSize(const QSize &size)
{
    setFixedSize(size);
    mSize = size;
}

QSize InsightLensWidget::lensSize() const
{
    return mSize;
}

void InsightLensWidget::setLensZoomLevel(int percent)
{
    mZoomLevel = percent;
    moveLensContentToMousePosition();
}

int InsightLensWidget::lensZoomLevel() const
{
    return mZoomLevel;
}

void InsightLensWidget::setMouseTracking(bool enable)
{
    mMouseTracking = enable;
    moveLensToMousePosition();
    moveLensContentToMousePosition();
}

bool InsightLensWidget::mouseTracking() const
{
    return mMouseTracking;
}

bool InsightLensWidget::toggleMouseTracking()
{
    setMouseTracking(!mMouseTracking);
    return mMouseTracking;
}

void InsightLensWidget::moveLensToMousePosition()
{
    if (mBuddyView && mEnabled && mMouseTracking) {
        // center widget at the mouse cursor position
        // (in the buddy view)
        QPoint pos = mBuddyView->mapFromGlobal(QCursor::pos());
        pos -= QPoint(width()/2, height()/2);
        move(pos);
    }
}

void InsightLensWidget::moveLensContentToMousePosition()
{
    if (mEnabled && mBuddyView) {
        // Center our local view at the same scene position as the mouse
        // cursor is in the buddy view
        QPoint pos = mBuddyView->mapFromGlobal(QCursor::pos());
        QTransform t = mBuddyView->viewportTransform();
        pos = t.inverted().map(pos);
        mView->setTransform(t*mZoomLevel/100.0);
        mView->centerOn(pos);
    }
}

bool InsightLensWidget::isLensEnabled() const
{
    return mEnabled;
}

void InsightLensWidget::setLensEnabled(bool enable)
{
    mEnabled = enable;
    setVisible(mEnabled);
    moveLensToMousePosition();
    moveLensContentToMousePosition();
    fixupPaintingArtifacts();
}

bool InsightLensWidget::toggleLensEnabled()
{
    setLensEnabled(!mEnabled);
    return mEnabled;
}

void InsightLensWidget::setBuddyView(QGraphicsView *view)
{
    if (mBuddyView) {
        mBuddyView->viewport()->removeEventFilter(this);
        mBuddyView->removeEventFilter(this);
    }
    mBuddyView = view;
    mView->setScene(mBuddyView->scene());
    mBuddyView->installEventFilter(this);
    mBuddyView->viewport()->installEventFilter(this);
    moveLensToMousePosition();
    moveLensContentToMousePosition();
}

QGraphicsView *InsightLensWidget::buddyView() const
{
    return mBuddyView;
}

bool InsightLensWidget::eventFilter(QObject *watched, QEvent *event)
{

    if (mBuddyView && (watched == mBuddyView->viewport() ||
                       watched == mBuddyView)) {
        // Update ourself when something happens on the buddy view
        moveLensToMousePosition();
        moveLensContentToMousePosition();
        // Filter out wheel events if needed
        if (event->type() == QEvent::Wheel) {
            QWheelEvent *wev = static_cast<QWheelEvent*>(event);
            return handleWheelEvent(wev);
        }
    }
    return QWidget::eventFilter(watched, event);
}

void InsightLensWidget::paintEvent(QPaintEvent *event)
{
    if (mShape == RoundLens) {
        QPainter p(this);
        // The rect will be clipped by the mask()
        // Note: this is not the cause of the painting artifacts
        p.fillRect(rect(), palette().color(QPalette::Foreground));
    }
    else
        QFrame::paintEvent(event);
}

// Zoom in/out only if the event happens above us and has the right modifiers
bool InsightLensWidget::handleWheelEvent(QWheelEvent *event)
{
    QPoint pos = mapFromGlobal(event->globalPos());
    // Qt::AltModifier is never set, why?
    if (rect().contains(pos) &&
            event->modifiers() & Qt::ShiftModifier) {
        qreal factor;
        if (event->delta() > 0)
            factor = 1.2;
        else
            factor = 0.8;
        setLensZoomLevel(lensZoomLevel()*factor);
        return true;
    }
    else
        return false;
}

// Quick and dirty painting artifact fix:
// This allow to erase the rectangle frame border when switching from
// SquareLens to RoundLens, (and fixes other tricky artifacts too!)
// This is used in setLensEnable() and setLensShape()
void InsightLensWidget::fixupPaintingArtifacts()
{
    setVisible(false);
    repaint();
    if (mEnabled)
        setVisible(true);
}

