#include "gridmanager.h"
#include "igrid.h"
#include "igrideditor.h"
#include "igridfactory.h"

static GridManager *mManager = 0;

GridManager::GridManager(QObject *parent) :
    QObject(parent)
{
}

GridManager *GridManager::instance()
{
    if (!mManager)
        mManager = new GridManager;
    return mManager;
}

GridManager::~GridManager()
{
    qDeleteAll(mGrids);
    qDeleteAll(mFactories);
    mManager = 0;
}

void GridManager::registerFactory(const QString &typeId, IGridFactory *factory)
{
    if (mFactories.keys().contains(typeId))
        return;
    mFactories.insert(typeId, factory);
}

IGrid *GridManager::createGrid(const QString &typeId) const
{
    if (!mFactories.keys().contains(typeId))
        return 0;
    return mFactories[typeId]->createGrid();
}

IGridEditor *GridManager::createGridEditor(const QString &typeId) const
{
    if (!mFactories.keys().contains(typeId))
        return 0;
    return mFactories[typeId]->createGridEditor();
}

QList<QString> GridManager::gridTypeList() const
{
    QList<QString> list;
    foreach (QString id, mFactories.keys())
        list.append(id);
    return list;
}

QString GridManager::gridTypeLabel(const QString &typeId) const
{
    if (!mFactories.keys().contains(typeId))
        return QStringLiteral("Unknown");
    return mFactories[typeId]->label();
}

QList<const IGrid *> GridManager::grids()
{
    QList<const IGrid *> list;
    foreach (IGrid *grid, mGrids)
        list.append(grid);
    return list;
}

void GridManager::setGrids(const QList<const IGrid *> &grids)
{
    QList<IGrid *> olds = mGrids;
    mGrids.clear();
    foreach (const IGrid *grid, grids)
        mGrids.append(grid->clone());
    emit gridsChanged();
    qDeleteAll(olds);
}
