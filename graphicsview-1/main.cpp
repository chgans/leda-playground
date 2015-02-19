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
    w->setGraphicsScene(&scene);
    w->show();


    {
        QGraphicsItemGroup *group = new QGraphicsItemGroup();
        group->setFlag(QGraphicsItem::ItemIsMovable, true);
        group->setFlag(QGraphicsItem::ItemIsSelectable, true);
        scene.addItem(group);

        QString filename("../../graphicsview-1/test.json");
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
        //int i = 0;
        QList<QGraphicsItem*> maskingItems;
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
            if (type.toLower() == "rectangle") {
                QVector<double> pos = readPosition(obj["position"].toArray());
                QVector<QPointF> points = readPointList(obj["points"].toArray());
                QPen pen = readPen(obj["pen"].toObject());
                QBrush brush = readBrush(obj["brush"].toObject());
                double opacity = obj["opacity"].toDouble();
                QGraphicsRectItem *item = new QGraphicsRectItem();
                item->setPos(pos[0], pos[1]);
                item->setZValue(pos[2]);
                item->setRect(QRectF(points[0], points[1]));
                item->setPen(pen);
                item->setBrush(brush);
                item->setOpacity(opacity);
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
                item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                scene.addItem(item);
                //group->addToGroup(item);
                //qDebug() << item;
                //static int i = 0;
                //if (++i == 3)
                //    w.mv->addMaskingItem(item);
            }
            else if (type.toLower() == "polygon") {
                QVector<double> pos = readPosition(obj["position"].toArray());
                QVector<QPointF> points = readPointList(obj["points"].toArray());
                QPen pen = readPen(obj["pen"].toObject());
                QBrush brush = readBrush(obj["brush"].toObject());
                double opacity = obj["opacity"].toDouble();
                QGraphicsPolygonItem *item = new QGraphicsPolygonItem();
                item->setPos(pos[0], pos[1]);
                item->setZValue(pos[2]);
                item->setPolygon(QPolygonF(points));
                item->setPen(pen);
                item->setBrush(brush);
                item->setOpacity(opacity);
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
                item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                scene.addItem(item);
                //group->addToGroup(item);
            }
            else if (type.toLower() == "simple-text") {
                QVector<double> pos = readPosition(obj["position"].toArray());
                QPen pen = readPen(obj["pen"].toObject());
                QBrush brush = readBrush(obj["brush"].toObject());
                double opacity = obj["opacity"].toDouble();
                QString text = obj["text"].toString();
                QFont font = readFont(obj["font"].toObject());
                QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem();
                item->setPos(pos[0], pos[1]);
                item->setZValue(pos[2]);
                item->setText(text);
                item->setFont(font);
                item->setPen(pen);
                item->setBrush(brush);
                item->setOpacity(opacity);
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
                item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                scene.addItem(item);
                //group->addToGroup(item);
            }
        }
    }

    QGraphicsPathItem *pathItem1 = new QGraphicsPathItem;
    QGraphicsPathItem *pathItem2 = new QGraphicsPathItem;
    QGraphicsPathItem *pathItem3 = new QGraphicsPathItem;
    QPainterPath path1, path2, path3;

    QPointF p0(0, 0), p1(10, 30), p2(40, 60), p3(100, 5), p4(120, -40);
    int fig = 1;
    if (fig == 0) {
        path1.moveTo(p0);
        path1.lineTo(p1);
        path1.lineTo(p2);
        path1.lineTo(p3);
        path1.lineTo(p4);
    } else if (fig == 1) {
        path1.moveTo(p0);
        path1.lineTo(p1);
        QPointF c(10, 90);
        qreal r = 10;
        qreal a = 180;
        qreal b = 90;
        path1.arcTo(QRectF(c, c).adjusted(-r, -r, r, r), a, b);
        path1.lineTo(p4);
    } else if (fig == 2) {
        path1.moveTo(p0);
        path1.lineTo(p1);
        path1.quadTo(p2, p3);
        path1.lineTo(p4);
    } else if (fig == 3) { // FIXME: radial sym doesn't work
        path1.moveTo(p0);
        path1.cubicTo(p1, p2, p3);
        path1.lineTo(p4);
    }
    //path1.closeSubpath();

    pathItem1->setPath(path1);
    pathItem1->setBrush(QBrush(Qt::darkMagenta));
    pathItem1->setPen(QPen(Qt::red, 1));
    pathItem1->setFlag(QGraphicsItem::ItemIsMovable, true);
    pathItem1->setFlag(QGraphicsItem::ItemIsSelectable, true);
    scene.addItem(pathItem1);
    qDebug() << path1;
    //qDebug() << path1.toReversed();

    path2 = radialSymetricPath(path1);; // path1.toReversed(); //
    pathItem2->setPath(path2);
    pathItem2->setBrush(QBrush(Qt::darkGreen));
    pathItem2->setFlag(QGraphicsItem::ItemIsMovable, true);
    pathItem2->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pathItem2->setPen(QPen(Qt::red, 1));
    //pathItem2->setPos(200, 0);
    scene.addItem(pathItem2);
    qDebug() << path2;

    path3 = axialSymetricPath(path1);
    pathItem3->setPath(path3);
    pathItem3->setBrush(QBrush(Qt::darkCyan));
    pathItem3->setFlag(QGraphicsItem::ItemIsMovable, true);
    pathItem3->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pathItem3->setPen(QPen(Qt::red, 1));
    //pathItem3->setPos(400, 0);
    scene.addItem(pathItem3);
    qDebug() << path3;

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
