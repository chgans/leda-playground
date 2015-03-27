#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit SplashScreen(const QPixmap &pixmap = QPixmap(),
                          Qt::WindowFlags f = 0);
    explicit SplashScreen(QWidget * parent,
                          const QPixmap &pixmap = QPixmap(),
                          Qt::WindowFlags f = 0);
    void drawContents(QPainter *painter);

    void setProductInformation(const QString &info);
    QString productInformation() const;

signals:

public slots:

private:
    QString mProductInformation;
};

#endif // SPLASHSCREEN_H
