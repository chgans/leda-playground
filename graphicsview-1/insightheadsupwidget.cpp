#include "insightheadsupwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QColor>
#include <QGridLayout>
#include <QGraphicsView>
#include <QVariant>
#include <QFont>
#include <QPainter>
#include <QGradient>
#include <QEvent>


// TODO:
// - InsightDisplayWidget -> Insight{HeadsUp,Popup,Panel}Widget
// - {Cursor,Layer,Snap,...}Listener
// - HeadsUpWidget position: corner or follow mouse
// - find out why we need to call layout()->update()
// - Display bug, X/Y location is clipped. Why?
// - wheel delta over item doesn't scroll view ?!?
// - Fix unit display (disabled only if both loc and delta qare disabled, enable if one of them enabled)
// - move setAttribute(Qt::WA_TransparentForMouseEvents, true) to another managing class ?

InsightHeadsUpWidget::InsightHeadsUpWidget(QWidget *parent) :
    QWidget(parent)
{
    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    // Location items
    QGridLayout *locationLayout = new QGridLayout;
    m_xCursorLocationLabel = new QLabel();
    m_xCursorLocationLabelBuddy = new QLabel("X:");
    m_yCursorLocationLabel = new QLabel();
    m_yCursorLocationLabelBuddy = new QLabel("Y:");
    m_xCursorDeltaLabel = new QLabel();
    m_xCursorDeltaLabelBuddy = new QLabel("dX:");
    m_yCursorDeltaLabel = new QLabel();
    m_yCursorDeltaLabelBuddy = new QLabel("dY:");
    m_xCursorUnitLabel = new QLabel("mm");
    m_yCursorUnitLabel = new QLabel("mm");
    locationLayout->addWidget(m_xCursorLocationLabelBuddy, 0, 0);
    locationLayout->addWidget(m_xCursorLocationLabel, 0, 1);
    locationLayout->addWidget(m_yCursorLocationLabelBuddy, 1, 0);
    locationLayout->addWidget(m_yCursorLocationLabel, 1, 1);
    locationLayout->addWidget(m_xCursorDeltaLabelBuddy, 0, 2);
    locationLayout->addWidget(m_xCursorDeltaLabel, 0, 3);
    locationLayout->addWidget(m_yCursorDeltaLabelBuddy, 1, 2);
    locationLayout->addWidget(m_yCursorDeltaLabel, 1, 3);
    locationLayout->addWidget(m_xCursorUnitLabel, 0, 4);
    locationLayout->addWidget(m_yCursorUnitLabel, 1, 4);
    QHBoxLayout *alignLayout = new QHBoxLayout();
    alignLayout->addLayout(locationLayout);
    alignLayout->addStretch();
    locationLayout->setHorizontalSpacing(3);
    mainLayout->addLayout(alignLayout);

    // Other items
    m_currentLayerLabel = new QLabel();
    mainLayout->addWidget(m_currentLayerLabel);
    m_snapInfoLabel = new QLabel();
    mainLayout->addWidget(m_snapInfoLabel);
    m_objectSummaryLabel = new QLabel();
    mainLayout->addWidget(m_objectSummaryLabel);
    m_shortcutInfoLabel = new QLabel();
    mainLayout->addWidget(m_shortcutInfoLabel);
    m_violationInfoLabel = new QLabel();
    mainLayout->addWidget(m_violationInfoLabel);
    m_netInfoLabel = new QLabel();
    mainLayout->addWidget(m_netInfoLabel);
    m_componentInfoLabel = new QLabel();
    mainLayout->addWidget(m_componentInfoLabel);

    // Set default settings
    QLinearGradient gradient;
    gradient.setColorAt(0, QColor(0, 0, 0));
    gradient.setColorAt(1, QColor(26,88,124));
    setBrush(QBrush(gradient));
    setPen(QPen(QColor(255, 255, 255), 1));
    setOpacity(0.25);
    setHoverOpacity(0.75);
    QFont font1 = font();
    font1.setBold(true);
    QFont font2 = font();
    font2.setBold(false);
    QColor color1(224, 222, 172);
    QColor color2(255, 255, 255);
    Items itemSet1 = CursorLocation | LastClickDelta;
    Items itemSet2 = SnapInformation | ObjectSummary | ShortcutInformation | ViolationInformation |
            NetInformation | ComponentInformation;
    setDisplayedItems(itemSet1);
    setDisplayedItemsHover(itemSet1 | itemSet2);
    setItemColor(itemSet1, color1);
    setItemColor(itemSet2, color2);
    setItemFont(itemSet1, font1);
    setItemFont(itemSet2, font2);
    updateItemWidgets();

    // Mouse events go through to the buddy view
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // Testing
#if 0
    setDisplayMode(HoverMode);
    setCursorDelta(QPointF(0.2, 100.9630458));
    setSnapInformation("Snap information goes here...");
    setObjectSummary("Object summary goes here...");
    setShortCutInformation("Shorcut 1\nShorcut 2\nShorcut 3");
    setViolationInformation("Violation information goes here...");
    setNetInformation("Net information goes here...");
    setComponentInformation("Component information goes here...");
#endif
}

void InsightHeadsUpWidget::setItemFont(Item item, const QFont &font)
{
    Items items = item;
    setItemFont(items, font);
}

void InsightHeadsUpWidget::setItemFont(Items items, const QFont &font)
{
    foreach (QWidget *widget, itemWidgets(items)) {
        widget->setFont(font);
    }
}

QFont InsightHeadsUpWidget::itemFont(InsightHeadsUpWidget::Item item) const
{
    foreach (QWidget *widget, itemWidgets(item)) {
        return widget->font();
    }
    return QFont();
}

void InsightHeadsUpWidget::setItemColor(InsightHeadsUpWidget::Item item, const QColor &color)
{
    Items items = item;
    setItemColor(items, color);
}

void InsightHeadsUpWidget::setItemColor(Items items, const QColor &color)
{
    foreach (QWidget *widget, itemWidgets(items)) {
        QPalette p = widget->palette();
        p.setColor(QPalette::Text, color);
        widget->setPalette(p);
    }
}

QColor InsightHeadsUpWidget::itemColor(InsightHeadsUpWidget::Item item) const
{
    foreach (QWidget *widget, itemWidgets(item)) {
        return widget->palette().color(QPalette::WindowText);
    }
    return QColor();
}

InsightHeadsUpWidget::Items InsightHeadsUpWidget::displayedItems() const
{
    return m_displayedItems;
}

void InsightHeadsUpWidget::setDisplayedItem(InsightHeadsUpWidget::Item item, bool displayed)
{
    if (m_displayedItems.testFlag(item) == displayed)
        return;

    m_displayedItems ^= item;
    updateItemWidgets();
    emit displayedItemsChanged();
}

void InsightHeadsUpWidget::setDisplayedItems(Items items)
{
    if (m_displayedItems == items)
        return;

    m_displayedItems = items;
    updateItemWidgets();
    emit displayedItemsChanged();
}

void InsightHeadsUpWidget::setDisplayedItemHover(InsightHeadsUpWidget::Item item, bool displayed)
{
    if (m_displayedItemsHover.testFlag(item) == displayed)
        return;
    m_displayedItemsHover |= item;
    updateItemWidgets();
    emit displayedItemsHoverChanged();
}

InsightHeadsUpWidget::Items InsightHeadsUpWidget::displayedItemsHover() const
{
    return m_displayedItemsHover;
}

bool InsightHeadsUpWidget::displayedItem(InsightHeadsUpWidget::Item item) const
{
    return m_displayedItems.testFlag(item);
}

bool InsightHeadsUpWidget::displayedItemHover(InsightHeadsUpWidget::Item item) const
{
    return m_displayedItemsHover.testFlag(item);
}

void InsightHeadsUpWidget::setDisplayedItemsHover(Items items)
{
    if (m_displayedItemsHover == items)
        return;

    m_displayedItemsHover = items;
    updateItemWidgets();
    emit displayedItemsHoverChanged();
}

qreal InsightHeadsUpWidget::opacity() const
{
    return m_opacity;
}

qreal InsightHeadsUpWidget::hoverOpacity() const
{
    return m_hoverOpacity;
}

qreal InsightHeadsUpWidget::effectiveOpacity() const
{
    if (m_displayMode == HoverMode)
        return m_hoverOpacity;
    else
        return m_opacity;
}

InsightHeadsUpWidget::DisplayMode InsightHeadsUpWidget::displayMode() const
{
    return m_displayMode;
}

void InsightHeadsUpWidget::setBuddyView(QGraphicsView *view)
{
    if (m_view != nullptr && m_view->scene() != nullptr)
        m_view->scene()->removeEventFilter(this);
    m_view = view;
    if (m_view != nullptr && m_view->scene() != nullptr)
        m_view->scene()->installEventFilter(this);
}

QGraphicsView *InsightHeadsUpWidget::buddyView() const
{
    return m_view;
}

QPen InsightHeadsUpWidget::pen() const
{
    return m_pen;
}

QBrush InsightHeadsUpWidget::brush() const
{
    return m_brush;
}

void InsightHeadsUpWidget::setItemData(InsightHeadsUpWidget::Item item, const QVariant &data)
{
    switch (item) {
    case CursorLocation:
        setCursorLocation(data.toPointF());
        break;
    case LastClickDelta:
        setCursorDelta(data.toPointF());
        break;
    case CurrentLayer:
        setCurrentLayer(data.toString());
        break;
    case SnapInformation:
        setSnapInformation(data.toString());
        break;
    case ObjectSummary:
        setObjectSummary(data.toString());
        break;
    case ShortcutInformation:
        setShortCutInformation(data.toString());
        break;
    case ViolationInformation:
        setViolationInformation(data.toString());
        break;
    case NetInformation:
        setNetInformation(data.toString());
        break;
    case ComponentInformation:
        setComponentInformation(data.toString());
        break;
    default:
        // Not reached
        break;
    }
}

void InsightHeadsUpWidget::resetDeltaOrigin()
{
    setCursorDeltaOrigin(m_cursorLocation);
    setCursorDelta(QPointF(0, 0));
}

void InsightHeadsUpWidget::setOpacity(qreal opacity)
{
    if (m_opacity == opacity)
        return;

    m_opacity = opacity;
    updateContent();
    emit opacityChanged(m_opacity);
}

void InsightHeadsUpWidget::setHoverOpacity(qreal opacity)
{
    if (m_hoverOpacity == opacity)
        return;

    m_hoverOpacity = opacity;
    updateContent();
    emit hoverOpacityChanged(opacity);
}

void InsightHeadsUpWidget::setDisplayMode(InsightHeadsUpWidget::DisplayMode arg)
{
    if (m_displayMode == arg)
        return;

    m_displayMode = arg;
    updateItemWidgets();
    emit displayModeChanged(arg);
}

void InsightHeadsUpWidget::setPen(const QPen &pen)
{
    if (m_pen == pen)
        return;

    m_pen = pen;
    update();
    emit penChanged();
}

void InsightHeadsUpWidget::setBrush(const QBrush &brush)
{
    if (m_brush == brush)
        return;

    m_brush = brush;
    update();
    emit brushChanged();
}

void InsightHeadsUpWidget::setCursorLocation(const QPointF &pos)
{
    if (m_cursorLocation == pos)
        return;

    m_cursorLocation = pos;
    m_xCursorLocationLabel->setText(QString("%1").arg(pos.x(), 0, 'f', 3));
    m_yCursorLocationLabel->setText(QString("%1").arg(pos.y(), 0, 'f', 3));
    updateContent();
}

void InsightHeadsUpWidget::setCursorDelta(const QPointF &pos)
{
    m_xCursorDeltaLabel->setText(QString("%1").arg(pos.x(), 0, 'f', 3));
    m_yCursorDeltaLabel->setText(QString("%1").arg(pos.y(), 0, 'f', 3));
    updateContent();
}

void InsightHeadsUpWidget::setCursorDeltaOrigin(const QPointF &pos)
{
    if (m_cursorDeltaOrigin == pos)
        return;

    m_cursorDeltaOrigin = pos;
    updateContent();
}

void InsightHeadsUpWidget::setCurrentLayer(const QString &text)
{
    m_currentLayerLabel->setText(text);
}

void InsightHeadsUpWidget::setSnapInformation(const QString &text)
{
    m_snapInfoLabel->setText(text);
}

void InsightHeadsUpWidget::setObjectSummary(const QString &text)
{
    m_objectSummaryLabel->setText(text);
}

void InsightHeadsUpWidget::setShortCutInformation(const QString &text)
{
    m_shortcutInfoLabel->setText(text);
}

void InsightHeadsUpWidget::setViolationInformation(const QString &text)
{
    m_violationInfoLabel->setText(text);
}

void InsightHeadsUpWidget::setNetInformation(const QString &text)
{
    m_netInfoLabel->setText(text);
}

void InsightHeadsUpWidget::setComponentInformation(const QString &text)
{
    m_componentInfoLabel->setText(text);
}

bool InsightHeadsUpWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_view->scene()) {
        QPoint viewPos;
        QPointF scenePos;
        switch (event->type()) {
        case QEvent::GraphicsSceneMouseMove:
            viewPos = m_view->viewport()->mapFromGlobal(QCursor::pos());
            scenePos = m_view->mapToScene(viewPos);
            setCursorLocation(scenePos);
            setCursorDelta(m_cursorLocation - m_cursorDeltaOrigin);
            if (m_view->scene()->itemAt(scenePos, m_view->transform()))
                setDisplayMode(HoverMode);
            else
                setDisplayMode(HeadsUpMode);
            break;
        case QEvent::GraphicsSceneMousePress:
            resetDeltaOrigin();
            break;
        default:
            break;
        }

    }
    return QWidget::eventFilter(watched, event);
}

void InsightHeadsUpWidget::updateContent()
{
    // ?!?
    setVisible(false);
    setVisible(true);
}

QList<QWidget *> InsightHeadsUpWidget::itemWidgets(InsightHeadsUpWidget::Items items) const
{
    QList<QWidget *> widgets;

    if (items.testFlag(CursorLocation))
        widgets << m_xCursorLocationLabel << m_yCursorLocationLabel
                << m_xCursorLocationLabelBuddy << m_yCursorLocationLabelBuddy
                << m_xCursorUnitLabel << m_yCursorUnitLabel;
    if (items.testFlag(LastClickDelta))
        widgets << m_xCursorDeltaLabel << m_yCursorDeltaLabel
                << m_xCursorDeltaLabelBuddy << m_yCursorDeltaLabelBuddy
                << m_xCursorUnitLabel << m_yCursorUnitLabel;
    if (items.testFlag(CurrentLayer))
        widgets << m_currentLayerLabel;
    if (items.testFlag(SnapInformation))
        widgets << m_snapInfoLabel;
    if (items.testFlag(ObjectSummary))
        widgets << m_objectSummaryLabel;
    if (items.testFlag(ShortcutInformation))
        widgets << m_shortcutInfoLabel;
    if (items.testFlag(ViolationInformation))
        widgets << m_violationInfoLabel;
    if (items.testFlag(NetInformation))
        widgets << m_netInfoLabel;
    if (items.testFlag(ComponentInformation))
        widgets << m_componentInfoLabel;
    return widgets;
}

void InsightHeadsUpWidget::updateItemWidget(InsightHeadsUpWidget::Item item)
{
    bool enabled = (m_displayedItems.testFlag(item) && m_displayMode == HeadsUpMode) ||
            (m_displayedItemsHover.testFlag(item) && m_displayMode == HoverMode);

    foreach (QWidget *widget, itemWidgets(item)) {
        widget->setVisible(enabled);
    }
    updateContent();
}

void InsightHeadsUpWidget::updateItemWidgets()
{
    updateItemWidget(CursorLocation);
    updateItemWidget(LastClickDelta);
    updateItemWidget(CurrentLayer);
    updateItemWidget(SnapInformation);
    updateItemWidget(ObjectSummary);
    updateItemWidget(ShortcutInformation);
    updateItemWidget(ViolationInformation);
    updateItemWidget(NetInformation);
    updateItemWidget(ComponentInformation);
}

void InsightHeadsUpWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(m_brush);
    painter.setPen(m_pen);
    painter.setOpacity(effectiveOpacity());
    painter.drawRect(rect().adjusted(0, 0, -m_pen.width(), -m_pen.width()));
}
