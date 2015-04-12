/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2014 Mattia Basaglia
    Copyright (C) 2014 Calle Laakkonen

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

#include "gradientslider.h"
#include "paintborder.h"

#include <QPainter>
#include <QStyleOptionSlider>
#include <QLinearGradient>

class GradientSlider::Private
{
public:
    QLinearGradient gradient;
    QBrush back;

    Private() :
        back(Qt::darkGray, Qt::DiagCrossPattern)
    {
        back.setTexture(QPixmap(QLatin1String(":/img/alpha-background.png")));
        gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
    }

};

GradientSlider::GradientSlider(QWidget *parent) :
    QSlider(Qt::Horizontal, parent), p(new Private)
{
}

GradientSlider::GradientSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent), p(new Private)
{
}

GradientSlider::~GradientSlider()
{
	delete p;
}

QBrush GradientSlider::background() const
{
    return p->back;
}

void GradientSlider::setBackground(const QBrush &bg)
{
    p->back = bg;
    update();
}

QGradientStops GradientSlider::colors() const
{
    return p->gradient.stops();
}

void GradientSlider::setColors(const QGradientStops &colors)
{
    p->gradient.setStops(colors);
    update();
}

QLinearGradient GradientSlider::gradient() const
{
    return p->gradient;
}

void GradientSlider::setGradient(const QLinearGradient &gradient)
{
    p->gradient = gradient;
    update();
}

void GradientSlider::setColors(const QVector<QColor> &colors)
{
    QGradientStops stops;
    stops.reserve(colors.size());

    double c = colors.size() - 1;
    if(c==0) {
        stops.append(QGradientStop(0, colors.at(0)));

    } else {
        for(int i=0;i<colors.size();++i) {
            stops.append(QGradientStop(i/c, colors.at(i)));
        }
    }
    setColors(stops);
}

void GradientSlider::setFirstColor(const QColor &c)
{
    QGradientStops stops = p->gradient.stops();
    if(stops.isEmpty())
        stops.push_back(QGradientStop(0.0, c));
    else
        stops.front().second = c;
    p->gradient.setStops(stops);

    update();
}

void GradientSlider::setLastColor(const QColor &c)
{
    QGradientStops stops = p->gradient.stops();
    if(stops.size()<2)
        stops.push_back(QGradientStop(1.0, c));
    else
        stops.back().second = c;
    p->gradient.setStops(stops);
    update();
}

QColor GradientSlider::firstColor() const
{
    QGradientStops s = colors();
    return s.empty() ? QColor() : s.front().second;
}

QColor GradientSlider::lastColor() const
{
    QGradientStops s = colors();
    return s.empty() ? QColor() : s.back().second;
}


void GradientSlider::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(orientation() == Qt::Horizontal)
        p->gradient.setFinalStop(1, 0);
    else
        p->gradient.setFinalStop(0, 1);

    painter.setPen(Qt::NoPen);
    painter.setBrush(p->back);
    painter.drawRect(1,1,geometry().width()-2,geometry().height()-2);
    painter.setBrush(p->gradient);
    painter.drawRect(1,1,geometry().width()-2,geometry().height()-2);

    paint_tl_border(painter,size(),palette().color(QPalette::Mid),0);
    /*paint_tl_border(painter,size(),palette().color(QPalette::Dark),1);

    paint_br_border(painter,size(),palette().color(QPalette::Light),1);*/
    paint_br_border(painter,size(),palette().color(QPalette::Midlight),0);

    QStyleOptionSlider opt_slider;
    initStyleOption(&opt_slider);
    opt_slider.subControls = QStyle::SC_SliderHandle;
    if (isSliderDown())
        opt_slider.state |= QStyle::State_Sunken;
    style()->drawComplexControl(QStyle::CC_Slider, &opt_slider, &painter, this);



    /*QStyleOptionFrameV3 opt_frame;
    opt_frame.init(this);
    opt_frame.frameShape = QFrame::StyledPanel;
    opt_frame.rect = geometry();
    opt_frame.state = QStyle::State_Sunken;

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.translate(-geometry().topLeft());
    style()->drawControl(QStyle::CE_ShapedFrame, &opt_frame, &painter, this);*/
}

