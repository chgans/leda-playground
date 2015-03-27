// TODO: Qt/Digia license header

#include "textframeitemeditor.h"

#include "textframeitem.h"

#include <QPointer>
#include <QGraphicsView>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QColorDialog>
#include <QFontDatabase>
#include <QFontComboBox>

#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include <QDebug>

class ColorAction : public QAction
{
    Q_OBJECT

public:
    ColorAction(QObject *parent);

    const QColor& color() const { return m_color; }
    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

private slots:
    void chooseColor();

private:
    QColor m_color;
};

ColorAction::ColorAction(QObject *parent):
    QAction(parent)
{
    setText(tr("Text Color"));
    setColor(Qt::black);
    connect(this, SIGNAL(triggered()), this, SLOT(chooseColor()));
}

void ColorAction::setColor(const QColor &color)
{
    if (color == m_color)
        return;
    m_color = color;
    QPixmap pix(24, 24);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.fillRect(pix.rect(), m_color);
    painter.setPen(m_color.darker());
    painter.drawRect(pix.rect().adjusted(0, 0, -1, -1));
    setIcon(pix);
}

void ColorAction::chooseColor()
{
    const QColor col = QColorDialog::getColor(m_color, 0);
    if (col.isValid() && col != m_color) {
        setColor(col);
        emit colorChanged(m_color);
    }
}

class TextFrameItemEditor;

class TextFrameItemEditorToolBar : public QToolBar
{
    Q_OBJECT
public:
    TextFrameItemEditorToolBar(QWidget *parent = 0);

signals:
    void horizontalAlignementChanged(Qt::Alignment alignment);
    void verticalAlignmentChanged(QTextCharFormat::VerticalAlignment alignment);
    void fontFamilyChanged(const QString &family);
    void fontSizeChanged(int size);
    void fontBoldChanged(bool bold);
    void fontItalicChanged(bool italic);
    void fontUnderlineChanged(bool underline);
    void fontOverlineChanged(bool overline);
    void foregroundColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);

public slots:
    void setFont(const QFont &font);
    void setHorizontalAlignment(Qt::Alignment alignment);
    void setVerticalAlignment(QTextCharFormat::VerticalAlignment alignment);
    void setForegroundColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

private slots:
    void halignmentActionTriggered(QAction *action);
    void valignmentActionTriggered(QAction *action);
    void sizeInputActivated(const QString &size);
    void fontInputChanged(const QFont &font);
#if 0
    void insertLink();
    void insertImage();
    void layoutDirectionChanged();
#endif



private:
    QAction *m_bold_action;
    QAction *m_italic_action;
    QAction *m_underline_action;
    QAction *m_valign_sup_action;
    QAction *m_valign_sub_action;
    QAction *m_align_left_action;
    QAction *m_align_center_action;
    QAction *m_align_right_action;
    QAction *m_align_justify_action;
    QAction *m_layoutDirectionAction;
    QAction *m_link_action;
    QAction *m_image_action;
    QAction *m_simplify_richtext_action;
    ColorAction *m_color_action;
    QFontComboBox *m_font_input;
    QComboBox *m_font_size_input;
    //QGraphicsView *m_view;
    //QPointer<TextFrameItem> m_item;
};

static QAction *createCheckableAction(const QIcon &icon, const QString &text,
                                      QObject *receiver, const char *slot,
                                      QObject *parent = 0)
{
    QAction *result = new QAction(parent);
    result->setIcon(icon);
    result->setText(text);
    result->setCheckable(true);
    result->setChecked(false);
    if (slot)
        QObject::connect(result, SIGNAL(triggered(bool)), receiver, slot);
    return result;
}

TextFrameItemEditorToolBar::TextFrameItemEditorToolBar(QWidget *parent) :
    QToolBar(parent),
    m_link_action(new QAction(this)),
    m_image_action(new QAction(this)),
    m_color_action(new ColorAction(this)),
    m_font_input(new QFontComboBox),
    m_font_size_input(new QComboBox)
{
    // Font & font size size combo box
    m_font_size_input->setEditable(false);
    const QList<int> font_sizes = QFontDatabase::standardSizes();
    foreach (int font_size, font_sizes)
        m_font_size_input->addItem(QString::number(font_size));
    connect(m_font_input, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(fontInputChanged(QFont)));
    connect(m_font_size_input, SIGNAL(activated(QString)),
            this, SLOT(sizeInputActivated(QString)));
    addWidget(m_font_input);
    addWidget(m_font_size_input);

    addSeparator();

    // Bold, italic and underline buttons

    m_bold_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textbold.png")),
            tr("Bold"), this, SIGNAL(fontBoldChanged(bool)), this);
    m_bold_action->setShortcut(tr("CTRL+B"));
    addAction(m_bold_action);

    m_italic_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textitalic.png")),
            tr("Italic"), this, SIGNAL(fontItalicChanged(bool)), this);
    m_italic_action->setShortcut(tr("CTRL+I"));
    addAction(m_italic_action);

    m_underline_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textunder.png")),
            tr("Underline"), this, SIGNAL(fontUnderlineChanged(bool)), this);
    m_underline_action->setShortcut(tr("CTRL+U"));
    addAction(m_underline_action);

    addSeparator();

    // Left, center, right and justified alignment buttons
    QActionGroup *alignment_group = new QActionGroup(this);
    alignment_group->setExclusive(true);
    connect(alignment_group, SIGNAL(triggered(QAction*)),
                             SLOT(halignmentActionTriggered(QAction*)));

    m_align_left_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textleft.png")),
            tr("Left Align"), 0, 0, alignment_group);
    addAction(m_align_left_action);

    m_align_center_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textcenter.png")),
            tr("Center"), 0, 0, alignment_group);
    addAction(m_align_center_action);

    m_align_right_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textright.png")),
            tr("Right Align"), 0, 0, alignment_group);
    addAction(m_align_right_action);

    m_align_justify_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textjustify.png")),
            tr("Justify"), 0, 0, alignment_group);
    addAction(m_align_justify_action);

#if 0
    m_layoutDirectionAction = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/righttoleft.png")),
            tr("Right to Left"), this, SLOT(layoutDirectionChanged()));
    addAction(m_layoutDirectionAction);
#endif
    addSeparator();

    // Superscript and subscript buttons
    QActionGroup *valignment_group = new QActionGroup(this);
    valignment_group->setExclusive(true);
    connect(valignment_group, SIGNAL(triggered(QAction*)),
                             SLOT(valignmentActionTriggered(QAction*)));
    m_valign_sup_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textsuperscript.png")),
            tr("Superscript"), 0, 0, this);
    addAction(m_valign_sup_action);

    m_valign_sub_action = createCheckableAction(
            QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textsubscript.png")),
            tr("Subscript"), 0, 0, this);
    addAction(m_valign_sub_action);

    addSeparator();

    // Insert hyperlink and image buttons
#if 0
    m_link_action->setIcon(QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/textanchor.png")));
    m_link_action->setText(tr("Insert &Link"));
    connect(m_link_action, SIGNAL(triggered()), SLOT(insertLink()));
    addAction(m_link_action);

    m_image_action->setIcon(QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/insertimage.png")));
    m_image_action->setText(tr("Insert &Image"));
    connect(m_image_action, SIGNAL(triggered()), SLOT(insertImage()));
    addAction(m_image_action);

    addSeparator();
#endif

    // Text color button
    connect(m_color_action, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(foregroundColorChanged(QColor)));
    addAction(m_color_action);

    addSeparator();

#if 0
    // Simplify rich text
    m_simplify_richtext_action
        = createCheckableAction(QIcon(QStringLiteral(":/qt-project.org/designer/shared/images/simplifyrichtext.png")),
                                tr("Simplify Rich Text"), m_editor, SLOT(setSimplifyRichText(bool)));
    m_simplify_richtext_action->setChecked(m_editor->simplifyRichText());
    connect(m_editor, SIGNAL(simplifyRichTextChanged(bool)),
            m_simplify_richtext_action, SLOT(setChecked(bool)));
    addAction(m_simplify_richtext_action);
#endif
#if 0
    connect(editor, SIGNAL(textChanged()), this, SLOT(updateActions()));
    connect(editor, SIGNAL(stateChanged()), this, SLOT(updateActions()));
#else
    //connect(m_item->document(), SIGNAL(contentsChanged()),
    //        this, SLOT(updateActions()));
    //connect(m_item->document(), SIGNAL(cursorPositionChanged(QTextCursor)),
    //       this, SLOT(updateActions()));
    //connect(m_item.data(), SIGNAL(cursorPositionChanged()),
    //        this, SLOT(updateActions()));
#endif
    //updateActions();
}

void TextFrameItemEditorToolBar::halignmentActionTriggered(QAction *action)
{
    Qt::Alignment new_alignment;

    if (action == m_align_left_action) {
        new_alignment = Qt::AlignLeft;
    } else if (action == m_align_center_action) {
        new_alignment = Qt::AlignCenter;
    } else if (action == m_align_right_action) {
        new_alignment = Qt::AlignRight;
    } else {
        new_alignment = Qt::AlignJustify;
    }
    emit horizontalAlignementChanged(new_alignment);
}

void TextFrameItemEditorToolBar::valignmentActionTriggered(QAction *action)
{
    QTextCharFormat::VerticalAlignment new_alignment;
    QAction *other;
    if (action == m_valign_sub_action) {
        new_alignment = QTextCharFormat::AlignSubScript;
        other = m_valign_sup_action;
    }
    else {
        new_alignment = QTextCharFormat::AlignSuperScript;
        other = m_valign_sub_action;
    }
    if (action->isChecked()) {
        emit verticalAlignmentChanged(new_alignment);
        other->setChecked(false);
    }
    else
        emit verticalAlignmentChanged(QTextCharFormat::AlignNormal);
}

void TextFrameItemEditorToolBar::sizeInputActivated(const QString &size)
{
    bool ok;
    int i = size.toInt(&ok);
    if (!ok)
        return;
    emit fontSizeChanged(i);
}

void TextFrameItemEditorToolBar::fontInputChanged(const QFont &newfont)
{
    emit fontFamilyChanged(newfont.family());
}

void TextFrameItemEditorToolBar::setHorizontalAlignment(Qt::Alignment alignment)
{

    if (alignment & Qt::AlignLeft) {
        m_align_left_action->setChecked(true);
    } else if (alignment & Qt::AlignRight) {
        m_align_right_action->setChecked(true);
    } else if (alignment & Qt::AlignHCenter) {
        m_align_center_action->setChecked(true);
    } else {
        m_align_justify_action->setChecked(true);
    }
}

void TextFrameItemEditorToolBar::setVerticalAlignment(QTextCharFormat::VerticalAlignment alignment)
{
    m_valign_sub_action->setChecked(alignment == QTextCharFormat::AlignSubScript);
    m_valign_sup_action->setChecked(alignment == QTextCharFormat::AlignSuperScript);
}

void TextFrameItemEditorToolBar::setForegroundColor(const QColor &color)
{
    m_color_action->setColor(color);
}

void TextFrameItemEditorToolBar::setBackgroundColor(const QColor &color)
{
    Q_UNUSED(color);
}

#if 0
void TextFrameItemEditorToolBar::insertLink()
{
    //AddLinkDialog linkDialog(m_editor, this);
    //linkDialog.showDialog();
    //m_editor->setFocus();
}

void TextFrameItemEditorToolBar::insertImage()
{
#if 0
    const QString path = IconSelector::choosePixmapResource(m_core, m_core->resourceModel(), QString(), this);
    if (!path.isEmpty())
        m_editor->insertHtml(QStringLiteral("<img src=\"") + path + QStringLiteral("\"/>"));
#endif
}

void TextFrameItemEditorToolBar::layoutDirectionChanged()
{
    /*
    QTextCursor cursor = m_editor->textCursor();
    QTextBlock block = cursor.block();
    if (block.isValid()) {
        QTextBlockFormat format = block.blockFormat();
        const Qt::LayoutDirection newDirection = m_layoutDirectionAction->isChecked() ? Qt::RightToLeft : Qt::LeftToRight;
        if (format.layoutDirection() != newDirection) {
            format.setLayoutDirection(newDirection);
            cursor.setBlockFormat(format);
        }
    }
    */
}
#endif

void TextFrameItemEditorToolBar::setFont(const QFont &font)
{
    m_font_input->setCurrentFont(font);
    m_bold_action->setChecked(font.bold());
    m_italic_action->setChecked(font.italic());
    m_underline_action->setChecked(font.underline());
}

#if 0
void TextFrameItemEditorToolBar::updateActions()
{
    if (m_item == 0) {
        setEnabled(false);
        return;
    }

    const QTextCursor cursor = m_item->textCursor();
    const QTextBlockFormat blockFormat = cursor.blockFormat();
    const Qt::Alignment alignment = blockFormat.alignment();
    const QTextCharFormat charFormat = cursor.charFormat();
    const QFont font = charFormat.font();
    const QTextCharFormat::VerticalAlignment valign =
        charFormat.verticalAlignment();
    const bool superScript = valign == QTextCharFormat::AlignSuperScript;
    const bool subScript = valign == QTextCharFormat::AlignSubScript;

    if (alignment & Qt::AlignLeft) {
        m_align_left_action->setChecked(true);
    } else if (alignment & Qt::AlignRight) {
        m_align_right_action->setChecked(true);
    } else if (alignment & Qt::AlignHCenter) {
        m_align_center_action->setChecked(true);
    } else {
        m_align_justify_action->setChecked(true);
    }
    m_layoutDirectionAction->setChecked(cursor.blockFormat().layoutDirection() == Qt::RightToLeft);

    m_bold_action->setChecked(font.bold());
    m_italic_action->setChecked(font.italic());
    m_underline_action->setChecked(font.underline());
    m_valign_sup_action->setChecked(superScript);
    m_valign_sub_action->setChecked(subScript);

    const int size = font.pointSize();
    const int idx = m_font_size_input->findText(QString::number(size));
    if (idx != -1)
        m_font_size_input->setCurrentIndex(idx);
    m_font_input->setCurrentFont(font);

    m_color_action->setColor(charFormat.foreground().color());
}
#endif

TextFrameItemEditor::TextFrameItemEditor(QObject *parent) :
    QObject(parent),
    mItem(0),
    mView(0),
    mToolBar(new TextFrameItemEditorToolBar)
{
    mToolBar->hide();
    connect(mToolBar, SIGNAL(fontBoldChanged(bool)),
            this, SLOT(setFontBold(bool)));
    connect(mToolBar, SIGNAL(fontFamilyChanged(QString)),
            this, SLOT(setFontFamily(QString)));
    connect(mToolBar, SIGNAL(fontItalicChanged(bool)),
            this, SLOT(setFontItalic(bool)));
    connect(mToolBar, SIGNAL(fontOverlineChanged(bool)),
            this, SLOT(setFontOverLine(bool)));
    connect(mToolBar, SIGNAL(fontUnderlineChanged(bool)),
            this, SLOT(setFontUnderLine(bool)));
    connect(mToolBar, SIGNAL(fontSizeChanged(int)),
            this, SLOT(setFontSize(int)));
    connect(mToolBar, SIGNAL(foregroundColorChanged(QColor)),
            this, SLOT(setForegroundColor(QColor)));
    connect(mToolBar, SIGNAL(backgroundColorChanged(QColor)),
            this, SLOT(setBackgroundColor(QColor)));
    connect(mToolBar, SIGNAL(horizontalAlignementChanged(Qt::Alignment)),
            this, SLOT(setHorizontalAlignment(Qt::Alignment)));
    connect(mToolBar, SIGNAL(verticalAlignmentChanged(QTextCharFormat::VerticalAlignment)),
            this, SLOT(setVerticalAlignment(QTextCharFormat::VerticalAlignment)));
}

TextFrameItem *TextFrameItemEditor::attachedItem()
{
    return mItem;
}

void TextFrameItemEditor::attachItem(TextFrameItem *item, QGraphicsView *view)
{
    if (mItem)
        detachItem();
    mItem = item;
    mView = view;
    mToolBar->setParent(mView);
    updateToolBar();
    connect(mItem, SIGNAL(cursorPositionChanged()),
            this, SLOT(updateToolBar()));
    mToolBar->show();
}

void TextFrameItemEditor::detachItem()
{
    mToolBar->setParent(0);
    mToolBar->hide();
    connect(mItem, SIGNAL(cursorPositionChanged()),
            this, SLOT(updateToolBar()));
}

void TextFrameItemEditor::setFontFamily(const QString &family)
{
    QFont newfont = font();
    newfont.setFamily(family);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setFontSize(int size)
{
    QFont newfont = font();
    newfont.setPointSize(size);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setFontBold(bool bold)
{
    QFont newfont = font();
    newfont.setBold(bold);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setFontItalic(bool italic)
{
    QFont newfont = font();
    newfont.setItalic(italic);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setFontUnderLine(bool underline)
{
    QFont newfont = font();
    newfont.setUnderline(underline);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setFontOverLine(bool overline)
{
    QFont newfont = font();
    newfont.setOverline(overline);
    setFont(newfont);
    giveFocusToItem();
}

void TextFrameItemEditor::setHorizontalAlignment(Qt::Alignment alignment)
{
    QTextCursor cursor = mItem->textCursor();
    QTextBlockFormat fmt = cursor.blockFormat();
    fmt.setAlignment(alignment);
    cursor.setBlockFormat(fmt);
    mItem->setTextCursor(cursor);
    giveFocusToItem();
}

void TextFrameItemEditor::setVerticalAlignment(QTextCharFormat::VerticalAlignment alignment)
{
    QTextCursor cursor = mItem->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setVerticalAlignment(alignment);
    cursor.setCharFormat(fmt);
    mItem->setTextCursor(cursor);
    giveFocusToItem();
}

void TextFrameItemEditor::setForegroundColor(const QColor &color)
{
    QTextCursor cursor = mItem->textCursor();
    QTextBlockFormat fmt = cursor.blockFormat();
    fmt.setForeground(QBrush(color));
    cursor.setBlockFormat(fmt);
    mItem->setTextCursor(cursor);
    giveFocusToItem();
}

void TextFrameItemEditor::setBackgroundColor(const QColor &color)
{
    QTextCursor cursor = mItem->textCursor();
    QTextBlockFormat fmt = cursor.blockFormat();
    fmt.setBackground(QBrush(color));
    cursor.setBlockFormat(fmt);
    mItem->setTextCursor(cursor);
    giveFocusToItem();
}

void TextFrameItemEditor::giveFocusToItem()
{
    mView->setFocus();
    mItem->setFocus();
}

void TextFrameItemEditor::updateToolBar()
{
    mToolBar->setParent(mView);
    mToolBar->setFont(font());
    mToolBar->setHorizontalAlignment(horizontalAlignment());
    mToolBar->setVerticalAlignment(verticalAlignment());
    mToolBar->setForegroundColor(foregroundColor());
    mToolBar->setBackgroundColor(backgroundColor());
}

QFont TextFrameItemEditor::font() const
{
    return mItem->textCursor().charFormat().font();
}

Qt::Alignment TextFrameItemEditor::horizontalAlignment() const
{
    return mItem->textCursor().blockFormat().alignment();
}

QTextCharFormat::VerticalAlignment TextFrameItemEditor::verticalAlignment() const
{
    return mItem->textCursor().charFormat().verticalAlignment();
}

QColor TextFrameItemEditor::foregroundColor() const
{
    return mItem->textCursor().blockFormat().foreground().color();
}

QColor TextFrameItemEditor::backgroundColor() const
{
    return mItem->textCursor().blockFormat().background().color();
}

void TextFrameItemEditor::setFont(QFont &font)
{
    QTextCursor cursor = mItem->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFont(font);
    cursor.setCharFormat(fmt);
    mItem->setTextCursor(cursor);
}


#include "textframeitemeditor.moc"
