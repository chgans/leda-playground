#ifndef TEXTFRAMEITEMEDITOR_H
#define TEXTFRAMEITEMEDITOR_H

#include <QObject>
#include <QTextCharFormat>

class TextFrameItem;
class QGraphicsView;

class TextFrameItemEditorToolBar;

class TextFrameItemEditor : public QObject
{
    Q_OBJECT

public:
    explicit TextFrameItemEditor(QObject *parent = 0);

    TextFrameItem *attachedItem();

    QFont font() const;
    Qt::Alignment horizontalAlignment() const;
    QTextCharFormat::VerticalAlignment verticalAlignment() const;
    QColor foregroundColor() const;
    QColor backgroundColor() const;

signals:

public slots:
    void attachItem(TextFrameItem *item, QGraphicsView *view);
    void detachItem();

    void setFont(QFont &font);
    void setFontFamily(const QString &family);
    void setFontSize(int size);
    void setFontBold(bool bold);
    void setFontItalic(bool italic);
    void setFontUnderLine(bool underline);
    void setFontOverLine(bool overline);
    void setHorizontalAlignment(Qt::Alignment alignment);
    void setVerticalAlignment(QTextCharFormat::VerticalAlignment alignment);
    void setForegroundColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

private:
    void giveFocusToItem();
    Q_SLOT void updateToolBar();

private:
    TextFrameItem *mItem;
    QGraphicsView *mView;
    TextFrameItemEditorToolBar *mToolBar;
};

#endif // TEXTFRAMEITEMEDITOR_H
