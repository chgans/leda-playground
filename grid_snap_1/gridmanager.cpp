#include "gridmanager.h"
#include "gridmanagerdialog.h"
#include "gridmanagerdialog_p.h"
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
    if (m_dialog != nullptr)
        delete m_dialog;
    if (m_model != nullptr)
        delete m_model;
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

void GridManager::execDialog()
{
    if (m_dialog == nullptr) {
        m_model = new GridTableModel();
        m_dialog = new GridManagerDialog();
        connect(m_dialog, SIGNAL(applyRequested()),
                this, SLOT(applyGridChanges()));
        connect(m_dialog, SIGNAL(accepted()),
                this, SLOT(applyGridChanges()));
        connect(m_dialog, SIGNAL(resetRequested()),
                this, SLOT(resetGridChanges()));
    }
    m_model->setGrids(grids());
    m_dialog->setModel(m_model);
    m_dialog->show();
}

void GridManager::applyGridChanges()
{
    setGrids(m_model->grids());
}

void GridManager::resetGridChanges()
{
    m_model->setGrids(grids());
}
