#ifndef INSIGHTHEADSUPWIDGET_H
#define INSIGHTHEADSUPWIDGET_H

#include <QWidget>
#include <QPen>
#include <QBrush>

class QGridLayout;
class QLabel;
class QGraphicsView;

class InsightHeadsUpWidget : public QWidget
{
    Q_OBJECT
    Q_FLAGS(Items Item)
    Q_ENUMS(DisplayMode)

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal hoverOpacity READ hoverOpacity WRITE setHoverOpacity NOTIFY hoverOpacityChanged)
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

    Q_PROPERTY(DisplayMode displayMode READ displayMode WRITE setDisplayMode NOTIFY displayModeChanged)
    Q_PROPERTY(Items displayedItems READ displayedItems WRITE setDisplayedItems NOTIFY displayedItemsChanged)
    Q_PROPERTY(Items displayedItemsHover READ displayedItemsHover WRITE setDisplayedItemsHover NOTIFY displayedItemsHoverChanged)

public:
    enum Item {
        CursorLocation       = 0x00000001,
        LastClickDelta       = 0x00000002,
        CurrentLayer         = 0x00000004,
        SnapInformation      = 0x00000008,
        ObjectSummary        = 0x00000010, // Tool/context dependent
        ShortcutInformation  = 0x00000020,
        ViolationInformation = 0x00000040,
        NetInformation       = 0x00000080,
        ComponentInformation = 0x00000100
    };
    Q_DECLARE_FLAGS(Items, Item)

    enum DisplayMode {
        HeadsUpMode,
        HoverMode
    };

    explicit InsightHeadsUpWidget(QWidget *parent = 0);

    void setItemFont(Item item, const QFont &font);
    void setItemFont(Items items, const QFont &font);
    QFont itemFont(Item item) const;
    void setItemColor(Item item, const QColor &color);
    void setItemColor(Items items, const QColor &color);
    QColor itemColor(Item item) const;

    Items displayedItems() const;
    Items displayedItemsHover() const;
    bool displayedItem(Item item) const;
    bool displayedItemHover(Item item) const;

    qreal opacity() const;
    qreal hoverOpacity() const;
    qreal effectiveOpacity() const;

    QPen pen() const;
    QBrush brush() const;

    DisplayMode displayMode() const;

    void setBuddyView(QGraphicsView *view);
    QGraphicsView *buddyView() const;

signals:
    void displayedItemsChanged();
    void displayedItemsHoverChanged();
    void opacityChanged(qreal arg);
    void hoverOpacityChanged(qreal arg);
    void displayModeChanged(DisplayMode arg);

    void penChanged();
    void brushChanged();

public slots:
    void setItemData(Item item, const QVariant &data);
    void setCursorLocation(const QPointF &pos);
    void setCursorDelta(const QPointF &pos);
    void setCursorDeltaOrigin(const QPointF &pos);
    void setCurrentLayer(const QString &text);
    void setSnapInformation(const QString &text);
    void setObjectSummary(const QString &text);
    void setShortCutInformation(const QString &text);
    void setViolationInformation(const QString &text);
    void setNetInformation(const QString &text);
    void setComponentInformation(const QString &text);

    void resetDeltaOrigin();

    void setOpacity(qreal opacity);
    void setHoverOpacity(qreal opacity);

    void setDisplayedItem(Item item, bool displayed);
    void setDisplayedItems(Items items);
    void setDisplayedItemHover(Item item, bool displayed);
    void setDisplayedItemsHover(Items items);
    void setDisplayMode(DisplayMode arg);

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void updateContent();

private:
    QGraphicsView *m_view = nullptr;
    Items m_displayedItems = 0;
    Items m_displayedItemsHover = 0;
    QPointF m_cursorLocation;
    QPointF m_cursorDeltaOrigin;
    QLabel *m_xCursorUnitLabel;
    QLabel *m_yCursorUnitLabel;
    QLabel *m_xCursorLocationLabel;
    QLabel *m_xCursorLocationLabelBuddy;
    QLabel *m_yCursorLocationLabel;
    QLabel *m_yCursorLocationLabelBuddy;
    QLabel *m_xCursorDeltaLabel;
    QLabel *m_xCursorDeltaLabelBuddy;
    QLabel *m_yCursorDeltaLabel;
    QLabel *m_yCursorDeltaLabelBuddy;
    QLabel *m_currentLayerLabel;
    QLabel *m_snapInfoLabel;
    QLabel *m_objectSummaryLabel;
    QLabel *m_shortcutInfoLabel;
    QLabel *m_violationInfoLabel;
    QLabel *m_netInfoLabel;
    QLabel *m_componentInfoLabel;
    qreal m_opacity = 0.25;
    qreal m_hoverOpacity = 0.75;
    DisplayMode m_displayMode = HeadsUpMode;

    QList<QWidget *> itemWidgets(InsightHeadsUpWidget::Items items) const;
    void updateItemWidget(Item item);
    void updateItemWidgets();
    QPen m_pen;
    QBrush m_brush;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(InsightHeadsUpWidget::Items)

#endif // INSIGHTHEADSUPWIDGET_H
