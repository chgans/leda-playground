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
#include "designlayermanager.h"

#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Libre EDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("graphicsview-1");

    PcbPaletteManager *paletteManager = PcbPaletteManager::instance();
    paletteManager->setPalettesPath(QDir::currentPath() + "/../graphicsview-1/");
    paletteManager->loadPalettes();

    DesignLayerManager *layerManager = DesignLayerManager::instance();
    // layerManager->setProfilePath();
    // layerManager->loadProfiles();
    layerManager->loadFromDefaults();

    PcbPalette *palette = paletteManager->activePalette();
    foreach (DesignLayer *layer, layerManager->allLayers()) {
        QColor color = palette->color(PcbPalette::ColorRole(layer->stackPosition() + 1));
        layer->setColor(color);
    }

    Scene scene(-5000, -5000, 10000, 10000); // um

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
            int layerIndex = obj["layer"].toInt();
            DesignLayer *layer = layerManager->layerAt(layerIndex);
            QColor color = layer->color();
            qDebug() << layer->name() << layer->stackPosition() << layer->color();
            if (type.toLower() == "rectangle") {
                QGraphicsRectItem *ritem = new QGraphicsRectItem();
                ritem->setRect(QRectF(points[0], points[1]));
                QPen pen;
                pen.setWidth(0);
                QBrush brush(color);
                ritem->setPen(pen);
                ritem->setBrush(brush);
                item = ritem;
            }
            else if (type.toLower() == "polyline") {
                QGraphicsPolygonItem *pitem = new QGraphicsPolygonItem();
                pitem->setPolygon(QPolygonF(points));
                QPen pen;
                pen.setWidth(obj["width"].toInt());
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                pitem->setPen(pen);
                item = pitem;
            }
            else {
                continue;
            }
            item->setPos(pos[0], pos[1]);
            item->setZValue(pos[2]);
            item->setOpacity(0.75);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            scene.addItemToLayer(item, layerIndex);
        }
        layerManager->enableOnlyUsedLayers();
    }

    MainWindow *w = new MainWindow;
    w->setGraphicsScene(&scene);
    w->show();
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
