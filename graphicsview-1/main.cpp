#include "mainwindow.h"
#include "mainview.h"
#include "scene.h"

#include <QApplication>

#include <QDebug>

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsItemGroup>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QColor>
#include <QRgb>
#include <QPen>
#include <QBrush>
#include <QFont>

#include <QtMath>
#include <QVector>

QVector<double> readPosition(const QJsonArray a)
{
    double x = 0.0, y = 0.0, z = 0.0;
    switch (a.size()) {
    case 3:
        z = a[2].toDouble();
    case 2:
        x = a[0].toDouble();
        y = a[1].toDouble();
        break;
    default:
        qDebug() << "Invalid position";
        break;
    }
    QVector<double> pos;
    pos << x << y << z;
    return pos;
}

QPen readPen(const QJsonObject &obj)
{
    QPen pen;
    pen.setWidth(obj["width"].toDouble());
    pen.setColor(QColor(obj["color"].toString()));
    return pen;
}

QBrush readBrush(const QJsonObject &obj)
{
    QBrush brush;
    brush.setColor(obj["color"].toString());
    brush.setStyle(Qt::SolidPattern);
    return brush;
}

QFont readFont(const QJsonObject &obj)
{
    QFont font;
    font.setFamily(obj["family"].toString());
    font.setPointSizeF(obj["size"].toDouble());
    font.setWeight(QFont::Bold);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setStrikeOut(false);
    return font;
}

QVector<QPointF> readPointList(const QJsonArray a)
{
    QVector<QPointF> points;
    foreach (QJsonValue val, a) {
        points << QPointF(val.toArray()[0].toDouble(),
                          val.toArray()[1].toDouble());
    }
    return points;
}

// Apply a radial symetry to path using the mid-point of the segment [p0,pn] and return the result.
QPainterPath radialSymetricPath(const QPainterPath &path)
{
    QPainterPath result;
    int n = path.elementCount();
    result = path.toReversed();
    result.setElementPositionAt(0,
                                path.elementAt(0).x,
                                path.elementAt(0).y);
    for (int i=1; i<(n-1); ++i) {
        int to = i, ref = to - 1, from1 = n - i, from2 = from1 - 1;
        qreal dx = path.elementAt(from2).x - path.elementAt(from1).x;
        qreal dy = path.elementAt(from2).y - path.elementAt(from1).y;
        result.setElementPositionAt(to,
                                    result.elementAt(ref).x - dx,
                                    result.elementAt(ref).y - dy);
    }
    result.setElementPositionAt(n-1,
                                path.elementAt(n-1).x,
                                path.elementAt(n-1).y);
    return result;
}

// Apply an axial symetry to path using the axis (p0,pn) and return the result
QPainterPath axialSymetricPath(const QPainterPath &path)
{
    QPainterPath result = path;
    int n = result.elementCount();
    QLineF axis(result.elementAt(0).x, result.elementAt(0).y,
               result.elementAt(n-1).x, result.elementAt(n-1).y);
    for (int i=1; i<(n-1); ++i) {
        QLineF line(result.elementAt(0).x, result.elementAt(0).y,
                    result.elementAt(i).x, result.elementAt(i).y);
        line.setAngle(axis.angle() + line.angleTo(axis));
        result.setElementPositionAt(i, line.p2().x(), line.p2().y());
    }
    return result;
}




#include "pcbpalettesettingsdialog.h"
#include "pcbpalettemanager.h"
#include "pcbpalettemanagerdialog.h"

#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PcbPaletteManager *mng = PcbPaletteManager::instance();
    mng->setPalettesPath(QDir::currentPath());
    mng->loadPalettes();

    Scene scene(-500, -500, 1000, 1000);
    MainWindow *w = new MainWindow;
    scene.addLayer("Top layer", QColor("#ff0000"));
    scene.addLayer("Bottom layer", QColor("#0000ff"));
    scene.addLayer("Mechanical 1", QColor("#ff00ff"));
    scene.addLayer("Mechanical 13", QColor("#ff00ff"));
    scene.addLayer("Mechanical 15", QColor("#008000"));
    scene.addLayer("Top overlay", QColor("#ffff00"));
    scene.addLayer("Bottom overlay", QColor("#808000"));
    scene.addLayer("Top paste", QColor("#808080"));
    scene.addLayer("Bottom paste", QColor("#800000"));
    scene.addLayer("Top solder", QColor("#800080"));
    scene.addLayer("Bottom solder", QColor("#ff00ff"));
    scene.addLayer("Drill guide", QColor("#800000"));
    scene.addLayer("Keep-Out layer", QColor("#ff00ff"));
    scene.addLayer("Drill drawing", QColor("#ff002a"));
    scene.addLayer("Multi-Layer", QColor("#c0c0c0"));
    scene.activateLayer(0);;
    w->setGraphicsScene(&scene);
    w->show();


    {
        QString filename("../graphicsview-1/test.json");
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Couldn't open" << filename;
            return 1;
        }
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (doc.isNull()) {
            qDebug() << "Doc is null";
            qDebug() << error.errorString() << error.offset;
            return 1;
        }
        if (!doc.isObject()) {
            qDebug() << "Doc is not an object!";
            return 1;
        }

        // Header
        QJsonObject obj = doc.object();
        qDebug() << obj["producer"].toString()
                <<  obj["version"].toString()
                <<  obj["author"].toString()
                <<  obj["license"].toString();

        // Graphical items
        if (!obj.keys().contains("items")) {
            qDebug() << "Document has no items";
            return 1;
        }
        QJsonArray items = obj["items"].toArray();
        foreach (QJsonValue val, items) {
            if (!val.isObject()) {
                qDebug() << "Item is not an object";
                continue;
            }

            obj = val.toObject();
            QString type = obj["type"].toString();
            if (type.isNull()) {
                qDebug() << "Item has no type";
                continue;
            }

            QGraphicsItem *item;
            QVector<double> pos = readPosition(obj["position"].toArray());
            QVector<QPointF> points = readPointList(obj["points"].toArray());
            QPen pen = readPen(obj["pen"].toObject());
            QBrush brush = readBrush(obj["brush"].toObject());
            double opacity = obj["opacity"].toDouble();
            int layer = obj["layer"].toInt();

            if (type.toLower() == "rectangle") {
                QGraphicsRectItem *ritem = new QGraphicsRectItem();
                ritem->setRect(QRectF(points[0], points[1]));
                ritem->setPen(pen);
                ritem->setBrush(brush);
                item = ritem;
            }
            else if (type.toLower() == "polygon") {
                QGraphicsPolygonItem *pitem = new QGraphicsPolygonItem();
                pitem->setPolygon(QPolygonF(points));
                pitem->setPen(pen);
                pitem->setBrush(brush);
                item = pitem;
            }
            else {
                continue;
            }

            item->setPos(pos[0], pos[1]);
            item->setZValue(pos[2]);
            item->setOpacity(opacity);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            scene.activateLayer(layer);
            scene.addItemToLayer(item);
        }
    }


    app.exec();
    delete w;
    return 0;
}
/*


QPainterPath: Element count=5
 -> MoveTo(x=0, y=0)
 -> CurveTo(x=40, y=50)
 -> CurveToData(x=80, y=100)
 -> CurveToData(x=130, y=100)
 -> LineTo(x=180, y=30)

QPainterPath: Element count=5
 -> MoveTo(x=180, y=30)
 -> LineTo(x=130, y=100)
 -> CurveTo(x=80, y=100)
 -> CurveToData(x=40, y=50)
 -> CurveToData(x=0, y=0)

QPainterPath: Element count=5
 -> MoveTo(x=0, y=0)
 -> LineTo(x=-40, y=-50)
 -> CurveTo(x=-80, y=-100)
 -> CurveToData(x=-130, y=-100)
 -> CurveToData(x=-180, y=-30)

 */
