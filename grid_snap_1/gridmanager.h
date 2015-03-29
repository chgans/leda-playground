#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QHash>

class IGridFactory;
class IGridEditor;
class IGrid;

class GridManagerDialog;
class GridTableModel;

class GridManager : public QObject
{
    Q_OBJECT

public:

    static GridManager *instance();
    ~GridManager();

    void registerFactory(const QString &typeId, IGridFactory *fatory);

    IGrid *createGrid(const QString &typeId) const;
    IGridEditor *createGridEditor(const QString &typeId) const;

    QList<QString> gridTypeList() const;
    QString gridTypeLabel(const QString &typeId) const;

    QList<const IGrid *> grids();
    void setGrids(const QList<const IGrid *> &grids);

public slots:
    void execDialog();

signals:
    void gridsChanged();

protected slots:
    void applyGridChanges();
    void resetGridChanges();

private:
    explicit GridManager(QObject *parent = 0);
    // TODO: Id class (QTC) and QHash
    QMap<QString, IGridFactory*> mFactories;
    QList<IGrid*> mGrids;
    GridManagerDialog *m_dialog = nullptr;
    GridTableModel *m_model = nullptr;
};

#endif // GRIDMANAGER_H
