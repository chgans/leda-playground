#include "insightlenswidget.h"

#include <QGraphicsView>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPaintEngine>

#include <QDebug>

InsightLensWidget::InsightLensWidget(QWidget *parent) :
    QFrame(parent),
    m_view(new QGraphicsView),
    m_buddyView(0)
{
    // Mouse events go through to the buddy view
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // Set up our own view
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setInteractive(false);

    // Setup the layout
    setLayout(new QVBoxLayout);
    layout()->setMargin(0);
    layout()->addWidget(m_view);

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
    m_shape = shape;

    // Apply a mask in Round mode
    fixupPaintingArtifacts();
    if (m_shape == RoundLens) {
        setMask(QRegion(rect(), QRegion::Ellipse));
        // The view needs a smaller one to allow for border drawing
        int w = lineWidth();
        QRect r = rect().adjusted(w, w, -w, -w).translated(-w, -w);
        m_view->setMask(QRegion(r, QRegion::Ellipse));
    }
    else {
        m_view->clearMask();
        clearMask();
    }
}

void InsightLensWidget::toggleLensShape()
{
    if (m_shape == SquareLens)
        setLensShape(RoundLens);
    else
        setLensShape(SquareLens);
}

InsightLensWidget::LensShape InsightLensWidget::lensShape() const
{
    return m_shape;
}

void InsightLensWidget::setLensSize(const QSize &size)
{
    setFixedSize(size);
    m_size = size;
}

QSize InsightLensWidget::lensSize() const
{
    return m_size;
}

void InsightLensWidget::setLensZoomLevel(int percent)
{
    m_zoomLevel = percent;
    moveLensContentToMousePosition();
}

int InsightLensWidget::lensZoomLevel() const
{
    return m_zoomLevel;
}

void InsightLensWidget::setMouseTracking(bool enable)
{
    m_mouseTracking = enable;
    moveLensToMousePosition();
    moveLensContentToMousePosition();
}

bool InsightLensWidget::mouseTracking() const
{
    return m_mouseTracking;
}

bool InsightLensWidget::toggleMouseTracking()
{
    setMouseTracking(!m_mouseTracking);
    return m_mouseTracking;
}

void InsightLensWidget::moveLensToMousePosition()
{
    if (m_buddyView && m_enabled && m_mouseTracking) {
        // center widget at the mouse cursor position
        // (in the buddy view)
        QPoint pos = m_buddyView->mapFromGlobal(QCursor::pos());
        pos -= QPoint(width()/2, height()/2);
        move(pos);
    }
}

void InsightLensWidget::moveLensContentToMousePosition()
{
    if (m_enabled && m_buddyView) {
        // Center our local view at the same scene position as the mouse
        // cursor is in the buddy view
        QPoint pos = m_buddyView->mapFromGlobal(QCursor::pos());
        QTransform t = m_buddyView->viewportTransform();
        pos = t.inverted().map(pos);
        m_view->setTransform(t*m_zoomLevel/100.0);
        m_view->centerOn(pos);
    }
}

bool InsightLensWidget::isLensEnabled() const
{
    return m_enabled;
}

void InsightLensWidget::setLensEnabled(bool enable)
{
    m_enabled = enable;
    setVisible(m_enabled);
    moveLensToMousePosition();
    moveLensContentToMousePosition();
    fixupPaintingArtifacts();
}

bool InsightLensWidget::toggleLensEnabled()
{
    setLensEnabled(!m_enabled);
    return m_enabled;
}

void InsightLensWidget::setBuddyView(QGraphicsView *view)
{
    if (m_buddyView) {
        m_buddyView->viewport()->removeEventFilter(this);
        m_buddyView->removeEventFilter(this);
    }
    m_buddyView = view;
    m_view->setScene(m_buddyView->scene());
    m_buddyView->installEventFilter(this);
    m_buddyView->viewport()->installEventFilter(this);
    moveLensToMousePosition();
    moveLensContentToMousePosition();
}

QGraphicsView *InsightLensWidget::buddyView() const
{
    return m_buddyView;
}

bool InsightLensWidget::eventFilter(QObject *watched, QEvent *event)
{

    if (m_buddyView && (watched == m_buddyView->viewport() ||
                       watched == m_buddyView)) {
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
    if (m_shape == RoundLens) {
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
    if (m_enabled)
        setVisible(true);
}

