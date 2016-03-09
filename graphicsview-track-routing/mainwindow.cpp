#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "linetrackelement.h"
#include "arctrackelement.h"
#include "tracknode.h"

class Item;
class Collision // DesignRuleViolation ?
{
public:
    const Item *offendingItem;
    QPointF offendingPoint;
    const Item *offendedItem;
    QPointF offendedPoint;
    qreal offence;
    qreal limit;
};

/*
 * For a given net:
 *  - Min track width
 *  - Max track width
 *  - Preferred track width
 *  - Min clearance
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene(this)),
    m_view(new GraphicsView)
{
    ui->setupUi(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    m_view->setScene(m_scene);
    setCentralWidget(m_view);
    statusBar()->showMessage("SHIFT+SPACEBAR to cycle through routing strategies, "
                             "SPACEBAR to switch b/w routing direction");

    /*
    TrackNode *p1 = new TrackNode();
    p1->setPos(100, 100);
    m_scene->addItem(p1);
    TrackNode *p2 = new TrackNode();
    p2->setPos(200, 100);
    m_scene->addItem(p2);
    TrackNode *p3 = new TrackNode();
    p3->setPos(200, 200);
    m_scene->addItem(p3);
    TrackNode *p4 = new TrackNode();
    p4->setPos(300, 200);
    m_scene->addItem(p4);
    TrackNode *p5 = new TrackNode();
    p5->setPos(400, 100);
    m_scene->addItem(p5);
    LineTrackElement *t1 = new LineTrackElement(p1, p2);
    m_scene->addItem(t1);
    LineTrackElement *t2 = new LineTrackElement(p2, p3);
    m_scene->addItem(t2);
    LineTrackElement *t3 = new LineTrackElement(p3, p4);
    m_scene->addItem(t3);
    ArcTrackElement *a1 = new ArcTrackElement(p4, p5);
    a1->setPos(300, 100);
    m_scene->addItem(a1);
    */

    TrackNode *n1, *n2;
    qreal clearance = 10;
    n1 = new TrackNode();
    n1->setPos(50, 50);
    m_scene->addItem(n1);
    n2 = new TrackNode();
    n2->setPos(250, 50);
    m_scene->addItem(n2);
    auto offender = new LineTrackElement(n1, n2);
    offender->setWidth(10);
    offender->setClearance(0);
    m_scene->addItem(offender);
    n1 = new TrackNode();
    n1->setPos(150, 0);
    m_scene->addItem(n1);
    n2 = new TrackNode();
    n2->setPos(150, 100);
    m_scene->addItem(n2);
    auto offended = new LineTrackElement(n1, n2);
    offended->setWidth(10);
    offended->setClearance(0);
    m_scene->addItem(offended);

    WalkAround(offender, offended, clearance);
}

QPainterPath growShape(const QPainterPath &path, qreal distance)
{
    QPainterPathStroker stroker(QPen(Qt::black, 2*distance));
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(path).united(path).simplified();
}

void MainWindow::WalkAround(const TrackElement *subject, const TrackElement *obstacle,
                            qreal clearance)
{
    QPainterPath subjectShape = subject->shape();
    QPainterPath obstacleShapeWithClearance = growShape(obstacle->shape(), clearance);
    if (!subjectShape.intersects(obstacleShapeWithClearance))
        return;

    QPainterPath obstacleShapeWithWalkaround = growShape(obstacleShapeWithClearance, subject->width());

    auto polygons = obstacleShapeWithWalkaround.subtracted(subjectShape).toSubpathPolygons();

//    Qt::GlobalColor obstacleColor = Qt::darkGray;
    Qt::GlobalColor solutionColors[2] = { Qt::darkBlue, Qt::darkCyan };
    for (int i=0; i<polygons.count(); i++)
    {
        QPolygonF polygon = polygons.at(i);
//        QGraphicsPolygonItem *polygonItem = m_scene->addPolygon(polygon);
//        polygonItem->setFlag(QGraphicsItem::ItemIsMovable, true);
//        polygonItem->setBrush(obstacleColor);
//        polygonItem->setPen(Qt::NoPen);
        QPainterPath solutionPath;
        solutionPath.addPolygon(polygon);
        solutionPath = solutionPath.subtracted(obstacleShapeWithClearance);
        solutionPath = solutionPath.united(subjectShape.subtracted(obstacleShapeWithClearance));
        QGraphicsPathItem *pathItem = m_scene->addPath(solutionPath);
        pathItem->setFlag(QGraphicsItem::ItemIsMovable, true);
        pathItem->setBrush(solutionColors[i]);
        pathItem->setPen(Qt::NoPen);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
