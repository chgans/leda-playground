#include "gridtablemodel.h"

#include "cartesiangrid.h"
#include "polargrid.h"

GridTableModel::GridTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    IGrid *g;
    g = new CartesianGrid;
    g->setPriority(1);
    g->setLabel("Global grid");
    g->setFineLineColor(QColor("#72d6c1"));
    g->setCoarseLineColor(QColor("#509687"));
    mGrids.append(g);
    g = new PolarGrid;
    g->setPriority(2);
    g->setLabel("Foo grid");
    g->setFineLineColor(QColor("#ee0e22"));
    g->setCoarseLineColor(QColor("#a70a18"));
    mGrids.append(g);
    g = new CartesianGrid;
    g->setPriority(4);
    g->setLabel("Bar/Baz grid");
    g->setFineLineColor(QColor("#4d4d5c"));
    g->setCoarseLineColor(QColor("#918d90"));
    mGrids.append(g);
}

int GridTableModel::rowCount(const QModelIndex &parent) const
{
    return mGrids.size();
}

int GridTableModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant GridTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= mGrids.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        int i = index.row();
        IGrid *g = mGrids.at(i);
        switch (index.column()) {
        case 0:
            return g->priority();
        case 1:
            return g->typeName();
        case 2:
            return g->label();
        case 3:
            return g->description();
        case 4:
            return g->fineLineColor().name();
        case 5:
            return g->coarseLineColor().name();
        case 6:
            return QVariant::fromValue<bool>(g->enabledForComponents());
        case 7:
            return QVariant::fromValue<bool>(g->enabledForComponents());
        default:
            return QVariant();
        }
    } /*else if (role == Qt::EditRole) {
        int i = index.row();
        IGrid *g = mGrids.at(i);
        switch (index.column()) {
        case 6:
            return QVariant::fromValue<bool>(g->enabledForComponents());
        case 7:
            return QVariant::fromValue<bool>(g->enabledForComponents());
        default:
            return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 6:
        case 7:
            return Qt::AlignCenter;
        default:
            return QVariant();
        }
    }*/

    return QVariant();
}

QVariant GridTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QVariant(QLatin1Literal("Priority"));
        case 1:
            return QVariant(QLatin1Literal("Type"));
        case 2:
            return QVariant(QLatin1Literal("Name"));
        case 3:
            return QVariant(QLatin1Literal("Description"));
        case 4:
            return QVariant(QLatin1Literal("Fine"));
        case 5:
            return QVariant(QLatin1Literal("Coarse"));
        case 6:
            return QVariant(QLatin1Literal("Non comp."));
        case 7:
            return QVariant(QLatin1Literal("Comp."));
        default:
            return QVariant();
        }
    }
    else
        return QVariant();
}

bool GridTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (value.canConvert<bool>()) {
        mGrids.at(index.row())->setEnabledForComponents(value.toBool());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags GridTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = Qt::ItemIsSelectable|Qt::ItemIsEnabled;
    if (index.column() == 6 || index.column() == 7)
        f |= Qt::ItemIsEditable;
    return f;
}

IGrid *GridTableModel::grid(const QModelIndex &index)
{
    if (!index.isValid() || index.row() >= mGrids.size())
        return 0;
    return mGrids[index.row()];
}

void GridTableModel::reload()
{
    emit dataChanged(createIndex(0, 0),
                     createIndex(mGrids.size(), 7));
}


