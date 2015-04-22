#include "splashscreen.h"

#include <QPainter>
#include <QProgressBar>
#include <QBitmap>
#include <QDebug>

SplashScreen::SplashScreen(const QPixmap &pixmap, Qt::WindowFlags f):
    QSplashScreen(pixmap, f),
    m_progressBar(new QProgressBar(this))
{
    m_titleFont.setPointSize(32);
    m_titleFont.setBold(true);
    m_messageFont.setPointSize(14);
    m_titleFont.setItalic(true);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
    m_progressBar->setValue(0);
    setMask(pixmap.mask());
}


// #0e5177
// #c3d5df
void SplashScreen::drawContents(QPainter *painter)
{
    painter->setRenderHint(QPainter::TextAntialiasing);
    QPixmap pix = pixmap();

    QString text = productInformation();
    QFontMetrics fontMetric(m_titleFont);
    int textWidth = fontMetric.width(text);
    int textHeight = fontMetric.height();
    QPoint textPos(pix.width()/2 - textWidth/2, pix.height() - textHeight * 3 - 5);
    int base = pix.height() - (textHeight + 5);

    m_progressBar->setGeometry(textPos.x(), textPos.y() + 5, textWidth, textHeight);
    m_progressBar->show();

    painter->setFont(m_titleFont);
    painter->setPen(QPen(QColor("#0ffffff")));
    painter->drawText(textPos, text);

    fontMetric = QFontMetrics(m_messageFont);
    text = message();
    textWidth = fontMetric.width(text);
    textHeight = fontMetric.height();
    textPos = QPoint(pix.width()/2 - textWidth/2, base - textHeight);

    painter->setPen(QPen(Qt::white));
    painter->setFont(m_messageFont);
    painter->drawText(textPos, text);
}

void SplashScreen::setProductInformation(const QString &info)
{
    mProductInformation = info;
}

QString SplashScreen::productInformation() const
{
    return mProductInformation;
}

void SplashScreen::setProgress(int percent)
{
    m_progressBar->setValue(percent);
    m_progressBar->update();
}
