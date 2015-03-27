#include "splashscreen.h"

#include <QPainter>

#include <QDebug>

SplashScreen::SplashScreen(const QPixmap &pixmap, Qt::WindowFlags f):
    QSplashScreen(pixmap, f)
{

}

SplashScreen::SplashScreen(QWidget *parent, const QPixmap &pixmap, Qt::WindowFlags f):
    QSplashScreen(parent, pixmap, f)
{

}

void SplashScreen::drawContents(QPainter *painter)
{
    QPixmap pix = pixmap();
    pix.size();
    QFont font = painter->font();
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(24);
    painter->setFont(font);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(QPen(Qt::darkBlue));
    painter->drawText(QPoint(20, pix.size().height() - 20), productInformation());
    painter->setPen(QPen(Qt::black));
    font.setPointSize(14);
    painter->setFont(font);
    int base = painter->fontMetrics().height();
    foreach ( QString line, message().split("\n")) {
        painter->drawText(QPoint(20, base), line);
        base += painter->fontMetrics().height();
        //qDebug() << base;
    }
}

void SplashScreen::setProductInformation(const QString &info)
{
    mProductInformation = info;
}

QString SplashScreen::productInformation() const
{
    return mProductInformation;
}
