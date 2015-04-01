#include "insightheadsupwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QColor>
#include <QGridLayout>
#include <QGraphicsView>
#include <QVariant>
#include <QFont>

// TODO:
// - InsightDisplayWidget -> Insight{HeadsUp,Popup,Panel}Widget
// - {Cursor,Layer,Snap,...}Listener
// - HeadsUpWidget position: corner or follow mouse
// - background brush/gradiant and border color (Need to manage paint ourself?)
// - Don't assume the view is the parent widget

InsightHeadsUpWidget::InsightHeadsUpWidget(QWidget *parent) :
    QFrame(parent)
{
    // Widget style
    setFrameStyle(Box|Plain);
    setLineWidth(1);
    setAutoFillBackground(true);
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(26,88,124,122)); // Background color + opacity
    setPalette(p);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
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
    locationLayout->addWidget(m_xCursorLocationLabelBuddy, 0, 0);
    locationLayout->addWidget(m_xCursorLocationLabel, 0, 1);
    locationLayout->addWidget(m_yCursorLocationLabelBuddy, 1, 0);
    locationLayout->addWidget(m_yCursorLocationLabel, 1, 1);
    locationLayout->addWidget(m_xCursorDeltaLabelBuddy, 0, 2);
    locationLayout->addWidget(m_xCursorDeltaLabel, 0, 3);
    locationLayout->addWidget(m_yCursorDeltaLabelBuddy, 1, 2);
    locationLayout->addWidget(m_yCursorDeltaLabel, 1, 3);
    mainLayout->addLayout(locationLayout);

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

    // Set defaults
    QFont font1 = font();
    font1.setBold(true);
    QFont font2 = font();
    font1.setBold(true);
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

    // Mouse events go through to the buddy view but we still watch them
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_view = static_cast<QGraphicsView*>(parent);
    m_view->installEventFilter(this);
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

void InsightHeadsUpWidget::setDisplayedItems(Items items)
{
    if (m_displayedItems == items)
        return;

    m_displayedItems = items;
    updateItemWidgets();
    emit displayedItemsChanged();
}

InsightHeadsUpWidget::Items InsightHeadsUpWidget::displayedItemsHover() const
{
    return m_displayedItemsHover;
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

InsightHeadsUpWidget::DisplayMode InsightHeadsUpWidget::displayMode() const
{
    return m_displayMode;
}

QColor InsightHeadsUpWidget::color() const
{
    return m_color;
}

QColor InsightHeadsUpWidget::borderColor() const
{
    return m_borderColor;
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

}

void InsightHeadsUpWidget::setOpacity(qreal opacity)
{
    if (m_opacity == opacity)
        return;

    m_opacity = opacity;

    QPalette p = palette();
    QColor c = p.color(QPalette::Base);
    c.setAlphaF(m_opacity);
    p.setColor(QPalette::Base, c);
    setPalette(p);

    update();

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

void InsightHeadsUpWidget::setColor(QColor arg)
{
    if (m_color == arg)
        return;

    m_color = arg;
    emit colorChanged(arg);
}

void InsightHeadsUpWidget::setBorderColor(QColor arg)
{
    if (m_borderColor == arg)
        return;

    m_borderColor = arg;
    emit borderColorChanged(arg);
}

void InsightHeadsUpWidget::setCursorLocation(const QPointF &pos)
{
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
    if (watched == m_view) {
        QPoint viewPos = m_view->viewport()->mapFromGlobal(QCursor::pos());
        QPointF scenePos = m_view->mapToScene(viewPos);
        setCursorLocation(scenePos);
    }
    return QWidget::eventFilter(watched, event);
}

void InsightHeadsUpWidget::updateContent()
{
    layout()->update();
}

QList<QWidget *> InsightHeadsUpWidget::itemWidgets(InsightHeadsUpWidget::Items items) const
{
    QList<QWidget *> widgets;

    if (items.testFlag(CursorLocation))
        widgets << m_xCursorLocationLabel << m_yCursorLocationLabel
                << m_xCursorLocationLabelBuddy << m_yCursorLocationLabelBuddy;
    if (items.testFlag(LastClickDelta))
        widgets << m_xCursorDeltaLabel << m_yCursorDeltaLabel
                << m_xCursorDeltaLabelBuddy << m_yCursorDeltaLabelBuddy;
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
    bool enabled = m_displayedItems.testFlag(item) && m_displayMode == HeadsUpMode ||
            m_displayedItemsHover.testFlag(item) && m_displayMode == HoverMode;

    foreach (QWidget *widget, itemWidgets(item)) {
        widget->setVisible(enabled);
    }
}

void InsightHeadsUpWidget::updateItemWidgets()
{
    updateItemWidget(CursorLocation);
    updateItemWidget(CursorLocation);
    updateItemWidget(LastClickDelta);
    updateItemWidget(LastClickDelta);
    updateItemWidget(CurrentLayer);
    updateItemWidget(SnapInformation);
    updateItemWidget(ObjectSummary);
    updateItemWidget(ShortcutInformation);
    updateItemWidget(ViolationInformation);
    updateItemWidget(NetInformation);
    updateItemWidget(ComponentInformation);
}
