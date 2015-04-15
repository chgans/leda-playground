/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2014 Mattia Basaglia
    Copyright (C) 2014 Calle Laakkonen
    Copyright (C) 2015 Christian Gagneraud

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "colorwidget.h"
#include "gradientslider.h"
#include "hueslider.h"

#include <QApplication>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDesktopWidget>
#include <QMimeData>
#include <QPushButton>
#include <QScreen>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

// FIXME: remove the 2 streching here, this should be managed by the ColorWidget's user
// FIXME: Restore the pick color button, but update cursor icon
class ColorWidget::Private
{
public:
    bool pick_from_screen;
    bool alpha_enabled;

    Private() : pick_from_screen(false), alpha_enabled(true)
    {}

    void setupUi(ColorWidget *widget)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout;
        widget->setLayout(mainLayout);

//        QVBoxLayout *leftLayout = new QVBoxLayout;
//        mainLayout->addLayout(leftLayout);

        m_wheel = new ColorWheel();
        m_wheel->setWheelWidth(60);
        m_wheel->setSizePolicy(QSizePolicy::Fixed,
                               QSizePolicy::Fixed);
        mainLayout->addWidget(m_wheel, 0, Qt::AlignCenter);
        connect(m_wheel, &ColorWheel::colorSelected,
                widget, &ColorWidget::update_widgets);

        QGridLayout *rightLayout = new QGridLayout;
        mainLayout->addLayout(rightLayout);

        int row = 0;

        m_hueSlider = new HueSlider;
        m_hueSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_hueSlider, m_hueSpinBox,
                          new QLabel("Hue"), row++);
        connect(m_hueSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_hsv);

        m_saturationSlider = new GradientSlider;
        m_saturationSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_saturationSlider, m_saturationSpinBox,
                          new QLabel("Saturation"), row++);
        connect(m_saturationSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_hsv);

        m_valueSlider = new GradientSlider;
        m_valueSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_valueSlider, m_valueSpinBox,
                          new QLabel("Value"), row++);
        connect(m_valueSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_hsv);

        m_redSlider = new GradientSlider;
        m_redSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_redSlider, m_redSpinBox,
                          new QLabel("Red"), row++);
        connect(m_redSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_rgb);

        m_greenSlider = new GradientSlider;
        m_greenSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_greenSlider, m_greenSpinBox,
                          new QLabel("Green"), row++);
        connect(m_greenSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_rgb);

        m_blueSlider = new GradientSlider;
        m_blueSpinBox = new QSpinBox;
        initSliderSpinBox(rightLayout, m_blueSlider, m_blueSpinBox,
                          new QLabel("Blue"), row++);
        connect(m_blueSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::set_rgb);

        m_alphaSlider = new GradientSlider;
        m_alphaSpinBox = new QSpinBox();
        m_alphaLabel = new QLabel("Alpha");
        initSliderSpinBox(rightLayout, m_alphaSlider, m_alphaSpinBox,
                          m_alphaLabel, row++);
        connect(m_alphaSlider, &QAbstractSlider::valueChanged,
                widget, &ColorWidget::update_widgets);

        m_hexEdit = new QLineEdit;
        rightLayout->addWidget(new QLabel("Hex"), row, 0);
        rightLayout->addWidget(m_hexEdit, row, 1, 1, 2);
        connect(m_hexEdit, &QLineEdit::editingFinished,
                widget, &ColorWidget::on_edit_hex_editingFinished);
        connect(m_hexEdit, &QLineEdit::textEdited,
                widget, &ColorWidget::on_edit_hex_textEdited);
        row++;

        m_preview = new ColorPreview();
        rightLayout->addWidget(new QLabel("Preview"), row, 0);
        rightLayout->addWidget(m_preview, row, 1, 1, 2);
        row++;

        // This will push all sliders onto the top
        rightLayout->addWidget(new QWidget, row, 0);
        rightLayout->setRowStretch(row, 10);
    }

    void initSliderSpinBox(QGridLayout *layout, QSlider *slider, QSpinBox *spinbox, QLabel *label, int row)
    {
        slider->setMaximum(255);
        slider->setOrientation(Qt::Horizontal);
        slider->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Maximum);
        spinbox->setMaximum(255);
        connect(slider, &QAbstractSlider::valueChanged,
                spinbox, &QSpinBox::setValue);
        void (QSpinBox::*SpinBoxvalueChanged)(int);
        connect(spinbox, SpinBoxvalueChanged,
                slider, &QAbstractSlider::setValue);
        layout->addWidget(label, row, 0);
        layout->addWidget(slider, row, 1);
        layout->addWidget(spinbox, row, 2);
    }

    ColorPreview *m_preview;
    ColorWheel *m_wheel;
    HueSlider *m_hueSlider;
    QSpinBox *m_hueSpinBox;
    GradientSlider *m_saturationSlider;
    QSpinBox *m_saturationSpinBox;
    GradientSlider *m_valueSlider;
    QSpinBox *m_valueSpinBox;
    GradientSlider *m_redSlider;
    QSpinBox *m_redSpinBox;
    GradientSlider *m_greenSlider;
    QSpinBox *m_greenSpinBox;
    GradientSlider *m_blueSlider;
    QSpinBox *m_blueSpinBox;
    QLabel *m_alphaLabel;
    GradientSlider *m_alphaSlider;
    QSpinBox *m_alphaSpinBox;
    QLineEdit *m_hexEdit;
};

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent), p(new Private)
{
    p->setupUi(this);

    setAcceptDrops(true);

    // Add "pick color" button
    //QPushButton *pickButton = p->m_buttonBox->addButton(tr("Pick"), QDialogButtonBox::ActionRole);
    //pickButton->setIcon(QIcon::fromTheme("color-picker"));

    connect(p->m_wheel,&ColorWheel::displayFlagsChanged,
            this, &ColorWidget::wheelFlagsChanged);
}

QSize ColorWidget::sizeHint() const
{
    return QSize(400,0);
}

ColorWheel::Display_Flags ColorWidget::wheelFlags() const
{
    return p->m_wheel->displayFlags();
}

QColor ColorWidget::color() const
{
    QColor col = p->m_wheel->color();
    if(p->alpha_enabled)
        col.setAlpha(p->m_alphaSlider->value());
    return col;
}

void ColorWidget::setColor(const QColor &c)
{
    p->m_preview->setComparisonColor(c);
    setColorInternal(c);
}

void ColorWidget::setColorInternal(const QColor &c)
{
    // Note. The difference between this method and setColor, is that setColor
    // sets the official starting color of the dialog, while this is used to update
    // the current color which might not be the final selected color.
    p->m_wheel->setColor(c);
    p->m_alphaSlider->setValue(c.alpha());
    update_widgets();
}

void ColorWidget::showColor(const QColor &c)
{
    setColor(c);
    show();
}

void ColorWidget::setWheelFlags(ColorWheel::Display_Flags flags)
{
    p->m_wheel->setDisplayFlags(flags);
}

void ColorWidget::setPreviewDisplayMode(ColorPreview::Display_Mode mode)
{
    p->m_preview->setDisplayMode(mode);
}

ColorPreview::Display_Mode ColorWidget::previewDisplayMode() const
{
    return p->m_preview->displayMode();
}

void ColorWidget::setAlphaEnabled(bool a)
{
    p->alpha_enabled = a;

    p->m_alphaLabel->setVisible(a);
    p->m_alphaSlider->setVisible(a);
    p->m_alphaSpinBox->setVisible(a);
}

bool ColorWidget::alphaEnabled() const
{
    return p->alpha_enabled;
}

//void ColorWidget::setButtonMode(Button_Mode mode)
//{
//    p->button_mode = mode;
//    QDialogButtonBox::StandardButtons btns;
//    switch(mode) {
//        case OkCancel: btns = QDialogButtonBox::Ok | QDialogButtonBox::Cancel; break;
//        case OkApplyCancel: btns = QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Reset; break;
//        case Close: btns = QDialogButtonBox::Close;
//    }
//    p->m_buttonBox->setStandardButtons(btns);
//}

//ColorWidget::Button_Mode ColorWidget::buttonMode() const
//{
//    return p->button_mode;
//}

void ColorWidget::update_widgets()
{
    bool blocked = signalsBlocked();
    blockSignals(true);
    foreach(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(true);

    QColor col = color();

    p->m_redSlider->setValue(col.red());
    p->m_redSpinBox->setValue(p->m_redSlider->value());
    p->m_redSlider->setFirstColor(QColor(0,col.green(),col.blue()));
    p->m_redSlider->setLastColor(QColor(255,col.green(),col.blue()));

    p->m_greenSlider->setValue(col.green());
    p->m_greenSpinBox->setValue(p->m_greenSlider->value());
    p->m_greenSlider->setFirstColor(QColor(col.red(),0,col.blue()));
    p->m_greenSlider->setLastColor(QColor(col.red(),255,col.blue()));

    p->m_blueSlider->setValue(col.blue());
    p->m_blueSpinBox->setValue(p->m_blueSlider->value());
    p->m_blueSlider->setFirstColor(QColor(col.red(),col.green(),0));
    p->m_blueSlider->setLastColor(QColor(col.red(),col.green(),255));

    p->m_hueSlider->setValue(qRound(p->m_wheel->hue()*360.0));
    p->m_hueSlider->setColorSaturation(p->m_wheel->saturation());
    p->m_hueSlider->setColorValue(p->m_wheel->value());
    p->m_hueSpinBox->setValue(p->m_hueSlider->value());

    p->m_saturationSlider->setValue(qRound(p->m_wheel->saturation()*255.0));
    p->m_saturationSpinBox->setValue(p->m_saturationSlider->value());
    p->m_saturationSlider->setFirstColor(QColor::fromHsvF(p->m_wheel->hue(),0,p->m_wheel->value()));
    p->m_saturationSlider->setLastColor(QColor::fromHsvF(p->m_wheel->hue(),1,p->m_wheel->value()));

    p->m_valueSlider->setValue(qRound(p->m_wheel->value()*255.0));
    p->m_valueSpinBox->setValue(p->m_valueSlider->value());
    p->m_valueSlider->setFirstColor(QColor::fromHsvF(p->m_wheel->hue(), p->m_wheel->saturation(),0));
    p->m_valueSlider->setLastColor(QColor::fromHsvF(p->m_wheel->hue(), p->m_wheel->saturation(),1));


    QColor apha_color = col;
    apha_color.setAlpha(0);
    p->m_alphaSlider->setFirstColor(apha_color);
    apha_color.setAlpha(255);
    p->m_alphaSlider->setLastColor(apha_color);
    p->m_alphaSpinBox->setValue(p->m_alphaSlider->value());


    p->m_hexEdit->setText(col.name());

    p->m_preview->setColor(col);

    blockSignals(blocked);
    foreach(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(false);

    emit colorChanged(col);
}

void ColorWidget::set_hsv()
{
    if ( !signalsBlocked() )
    {
        p->m_wheel->setColor(QColor::fromHsv(
                p->m_hueSlider->value(),
                p->m_saturationSlider->value(),
                p->m_valueSlider->value()
            ));
        update_widgets();
    }
}

void ColorWidget::set_rgb()
{
    if ( !signalsBlocked() )
    {
        QColor col(
                p->m_redSlider->value(),
                p->m_greenSlider->value(),
                p->m_blueSlider->value()
            );
        if (col.saturation() == 0)
            col = QColor::fromHsv(p->m_hueSlider->value(), 0, col.value());
        p->m_wheel->setColor(col);
        update_widgets();
    }
}


void ColorWidget::on_edit_hex_editingFinished()
{
    update_hex();

}

void ColorWidget::on_edit_hex_textEdited(const QString &arg1)
{
    int cursor = p->m_hexEdit->cursorPosition();
    update_hex();
    //edit_hex->blockSignals(true);
    p->m_hexEdit->setText(arg1);
    //edit_hex->blockSignals(false);
    p->m_hexEdit->setCursorPosition(cursor);
}

void ColorWidget::update_hex()
{
    QString xs = p->m_hexEdit->text().trimmed();
    xs.remove('#');

    if ( xs.isEmpty() )
        return;

    if ( xs.indexOf(QRegExp("^[0-9a-fA-f]+$")) == -1 )
    {
        QColor c(xs);
        if ( c.isValid() )
        {
            setColorInternal(c);
            return;
        }
    }

    if ( xs.size() == 3 )
    {
        p->m_redSlider->setValue(QString(2,xs[0]).toInt(0,16));
        p->m_greenSlider->setValue(QString(2,xs[1]).toInt(0,16));
        p->m_blueSlider->setValue(QString(2,xs[2]).toInt(0,16));
    }
    else
    {
        if ( xs.size() < 6 )
        {
            xs += QString(6-xs.size(),'0');
        }
        p->m_redSlider->setValue(xs.mid(0,2).toInt(0,16));
        p->m_greenSlider->setValue(xs.mid(2,2).toInt(0,16));
        p->m_blueSlider->setValue(xs.mid(4,2).toInt(0,16));

        if ( xs.size() == 8 )
            p->m_alphaSlider->setValue(xs.mid(6,2).toInt(0,16));
    }

    set_rgb();
}

//void ColorWidget::on_buttonBox_clicked(QAbstractButton *btn)
//{
//    QDialogButtonBox::ButtonRole role = p->m_buttonBox->buttonRole(btn);

//    switch(role) {
//    case QDialogButtonBox::AcceptRole:
//    case QDialogButtonBox::ApplyRole:
//        // Explicitly select the color
//        p->m_preview->setComparisonColor(color());
//        emit colorSelected(color());
//        break;

//    case QDialogButtonBox::ActionRole:
//        // Currently, the only action button is the "pick color" button
//        grabMouse(Qt::CrossCursor);
//        p->pick_from_screen = true;
//        break;

//    case QDialogButtonBox::ResetRole:
//        // Restore old color
//        setColorInternal(p->m_preview->comparisonColor());
//        break;

//    default: break;
//    }
//}

void ColorWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if ( event->mimeData()->hasColor() ||
         ( event->mimeData()->hasText() && QColor(event->mimeData()->text()).isValid() ) )
        event->acceptProposedAction();
}


void ColorWidget::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasColor() )
    {
        setColorInternal(event->mimeData()->colorData().value<QColor>());
        event->accept();
    }
    else if ( event->mimeData()->hasText() )
    {
        QColor col(event->mimeData()->text());
        if ( col.isValid() )
        {
            setColorInternal(col);
            event->accept();
        }
    }
}

namespace {
QColor get_screen_color(const QPoint &global_pos)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    WId id = QApplication::desktop()->winId();
    QImage img = QPixmap::grabWindow(id, global_pos.x(), global_pos.y(), 1, 1).toImage();
#else
    int screenNum = QApplication::desktop()->screenNumber(global_pos);
    QScreen *screen = QApplication::screens().at(screenNum);

    WId wid = QApplication::desktop()->winId();
    QImage img = screen->grabWindow(wid, global_pos.x(), global_pos.y(), 1, 1).toImage();
#endif

    return img.pixel(0,0);
}
}

void ColorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (p->pick_from_screen)
    {
        setColorInternal(get_screen_color(event->globalPos()));
        p->pick_from_screen = false;
        releaseMouse();
    }
}

void ColorWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (p->pick_from_screen)
    {
        setColorInternal(get_screen_color(event->globalPos()));
    }
}

