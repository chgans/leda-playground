#ifndef INSIGHTPICKLISTWIDGET_H
#define INSIGHTPICKLISTWIDGET_H

#include <QFrame>
#include <QSplitter>

class ObjectPreview;
class QListWidget;
class QListWidgetItem;
class QTableWidget;
class QTableWidgetItem;
class QGraphicsItem;
class QGraphicsScene;


class InsightPickListWidget : public QFrame
{
    Q_OBJECT
public:
    explicit InsightPickListWidget(QWidget *parent = 0);
    ~InsightPickListWidget();

    void setPickList(QGraphicsScene *scene, QList<QGraphicsItem*> list);


    //void setMovable(bool movable);
    //bool isMovable() const;

signals:
    void itemSelected(QGraphicsItem *item);

public slots:


protected slots:
    void activateTableItem(QTableWidgetItem *item);
    void selectItem(const QModelIndex &index);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    ObjectPreview *mView;
    QTableWidget *mTable;
    QPoint mMousePos;
};

#endif // INSIGHTPICKLISTWIDGET_H
