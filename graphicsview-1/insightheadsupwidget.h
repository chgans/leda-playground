#ifndef INSIGHTHEADSUPWIDGET_H
#define INSIGHTHEADSUPWIDGET_H

#include <QFrame>

class QGridLayout;
class QLabel;
class QGraphicsView;

class InsightHeadsUpWidget : public QFrame
{
    Q_OBJECT
    Q_FLAGS(Items Item)
    Q_ENUMS(DisplayMode)

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal hoverOpacity READ hoverOpacity WRITE setHoverOpacity NOTIFY hoverOpacityChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)

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

    qreal opacity() const;
    qreal hoverOpacity() const;
    QColor color() const;
    QColor borderColor() const;

    DisplayMode displayMode() const;

signals:
    void displayedItemsChanged();
    void displayedItemsHoverChanged();
    void opacityChanged(qreal arg);
    void hoverOpacityChanged(qreal arg);
    void displayModeChanged(DisplayMode arg);

    void colorChanged(QColor arg);

    void borderColorChanged(QColor arg);

public slots:
    void setItemData(Item item, const QVariant &data);
    void setCursorLocation(const QPointF &pos);
    void setCursorDelta(const QPointF &pos);
    void setCursorDeltaOrigin(const QPointF &pos); // TBD
    void setCurrentLayer(const QString &text);
    void setSnapInformation(const QString &text);
    void setObjectSummary(const QString &text);
    void setShortCutInformation(const QString &text);
    void setViolationInformation(const QString &text);
    void setNetInformation(const QString &text);
    void setComponentInformation(const QString &text);

    void resetDeltaOrigin(); // TBD

    void setOpacity(qreal opacity);
    void setHoverOpacity(qreal opacity);

    void setDisplayedItems(Items items);
    void setDisplayedItemsHover(Items items);
    void setDisplayMode(DisplayMode arg);

    void setColor(QColor arg);
    void setBorderColor(QColor arg);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void updateContent();

private:
    QGraphicsView *m_view;
    Items m_displayedItems = 0;
    Items m_displayedItemsHover = 0;
    QPointF m_cursorLocation;
    QPointF m_cursorDelta;
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
    QColor m_color;
    QColor m_borderColor;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(InsightHeadsUpWidget::Items)

#endif // INSIGHTHEADSUPWIDGET_H
