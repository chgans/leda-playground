/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2014 Mattia Basaglia

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
#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>

#include "widgets/colorpreview.h"
#include "widgets/colorwheel.h"

class ColorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true)
    Q_PROPERTY(ColorWheel::Display_Flags wheelFlags READ wheelFlags WRITE setWheelFlags NOTIFY wheelFlagsChanged)

public:
    explicit ColorWidget(QWidget *parent = 0);

    /**
     * Get currently selected color
     */
    QColor color() const;

    /**
     * Set the display mode for the color preview
     */
    void setPreviewDisplayMode(ColorPreview::Display_Mode mode);

    /**
     * Get the color preview diplay mode
     */
    ColorPreview::Display_Mode previewDisplayMode() const;

    /**
     * Set whether the color alpha channel can be edited.
     * If alpha is disabled, the selected color's alpha will always be 255.
     */
    void setAlphaEnabled(bool a);

    bool alphaEnabled() const;

    QSize sizeHint() const;

    ColorWheel::Display_Flags wheelFlags() const;

public slots:

    /**
     * Change color
     */
    void setColor(const QColor &c);

	/**
     * Set the current color and show the dialog
     */
    void showColor(const QColor &oldcolor);

    void setWheelFlags(ColorWheel::Display_Flags flags);

signals:
    /**
     * The current color was changed
     */
    void colorChanged(QColor);

    /**
     * The user selected the new color by pressing Ok/Apply
     */
    void colorSelected(QColor);

    void wheelFlagsChanged(ColorWheel::Display_Flags flags);

private slots:
    /// Update all the Ui elements to match the selected color
    void update_widgets();
    /// Update from HSV sliders
    void set_hsv();
    /// Update from RGB sliders
    void set_rgb();

    void on_edit_hex_editingFinished();
    void on_edit_hex_textEdited(const QString &arg1);

private:
    void update_hex();
    void setColorInternal(const QColor &color);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    class Private;
    Private * const p;
};

#endif // COLORWIDGET_H
