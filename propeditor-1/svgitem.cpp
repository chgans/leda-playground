#include "svgitem.h"

#include <QSvgRenderer>

SvgItem::SvgItem(QGraphicsItem *parent):
    DrawingItem(parent)
{
    setSharedRenderer(new QSvgRenderer(this));
}

QString SvgItem::fileName() const
{
    return m_fileName;
}

void SvgItem::setFileName(QString arg)
{
    if (m_fileName != arg) {
        m_fileName = arg;
        if (!renderer()->load(arg)) {
            qDebug() << Q_FUNC_INFO << "Failed to load" << arg;
        }
        else
            setSharedRenderer(renderer());
        emit fileNameChanged(arg);
    }
}
