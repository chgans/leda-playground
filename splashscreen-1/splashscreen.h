#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QFont>

class QProgressBar;

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit SplashScreen(const QPixmap &pixmap = QPixmap(),
                          Qt::WindowFlags f = 0);
    void drawContents(QPainter *painter);

    void setProductInformation(const QString &info);
    QString productInformation() const;

    void setProgress(int percent);

signals:

public slots:

private:
    QString mProductInformation;
    QProgressBar *m_progressBar;
    QFont m_titleFont;
    QFont m_messageFont;
};

#endif // SPLASHSCREEN_H
