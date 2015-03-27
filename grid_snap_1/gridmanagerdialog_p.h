#ifndef GRIDMANAGERDIALOG_P_H
#define GRIDMANAGERDIALOG_P_H

#include "igrid.h"

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QCheckBox>
#include <QPainter>
#include <QAbstractTableModel>
#include <QList>

#include <QDebug>

class CheckBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    CheckBoxDelegate(QObject *parent = 0):
        QStyledItemDelegate(parent)
    {
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        Q_UNUSED(option)
        Q_UNUSED(index)
        return new QCheckBox(parent);
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        bool value = index.model()->data(index).toBool();
        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
        checkBox->setChecked(value);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
        model->setData(index, QVariant::fromValue(checkBox->isChecked()));
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
    {
        Q_UNUSED(index)
        // Center the editor in the cell
        QRect geom = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                         editor->sizeHint(),
                                         option.rect);
        editor->setGeometry(geom);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const
    {
        // Don't paint anything
        Q_UNUSED(painter)
        Q_UNUSED(option)
        Q_UNUSED(index)
        return;
    }

};

class ColorDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    ColorDelegate(QObject *parent = 0):
        QStyledItemDelegate(parent)
    {
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        Q_UNUSED(option)
        Q_UNUSED(index)
        QWidget *editor = new QWidget(parent);
        editor->setAutoFillBackground(true);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QColor color = index.model()->data(index).value<QColor>();
        QPalette palette;
        palette.setColor(QPalette::Base, color);
        editor->setPalette(palette);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QColor color = editor->palette().color(QPalette::Base);
        model->setData(index, QVariant::fromValue(color));
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
    {
        Q_UNUSED(index)
        // Center the editor in the cell
        QRect geom = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                         option.rect.size()*0.75,
                                         option.rect);
        editor->setGeometry(geom);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const
    {
        // Don't paint anything
        Q_UNUSED(painter)
        Q_UNUSED(option)
        Q_UNUSED(index)
        return;
    }

};


#include <cartesiangrid.h>

class IGrid;

class GridTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GridTableModel(QObject *parent = 0) :
        QAbstractTableModel(parent)
    {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return mGrids.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return 8;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.row() >= mGrids.size())
            return QVariant();

        //qDebug() << __FUNCTION__ << index << role;
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
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
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

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (role != Qt::DisplayRole)
            return false;

        switch (index.column()) {
        case 0:
            mGrids.at(index.row())->setPriority(value.toInt());
            return true;
        case 7:
            mGrids.at(index.row())->setEnabledForComponents(value.toBool());
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags f = Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        if (index.column() == 6 || index.column() == 7)
            f |= Qt::ItemIsEditable;
        return f;
    }

    const IGrid *grid(const QModelIndex &index)
    {
        if (!index.isValid() || index.row() >= mGrids.size())
            return 0;
        return mGrids[index.row()];
    }

    void setGrids(const QList<const IGrid *> &grids)
    {
        beginResetModel();
        QList<IGrid *> olds = mGrids;
        mGrids.clear();
        foreach (const IGrid* grid, grids) {
            mGrids.append(grid->clone());
        }
        endResetModel();
        qDeleteAll(olds);
    }

    QList<const IGrid *> grids()
    {
        QList<const IGrid *> grids;
        foreach (IGrid *grid, mGrids) {
            grids.append(grid);
        }
        return grids;
    }

    // FIXME: manage prio
    void addGrid(IGrid *grid)
    {
        if (mGrids.contains(grid))
            return;
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        mGrids.append(grid);
        endInsertRows();
    }

    void setGrid(int row, IGrid *grid)
    {
        if (row >= mGrids.count())
            return;
        IGrid *old = mGrids[row];
        beginResetModel();
        mGrids[row] = grid;
        endResetModel();
        delete old;
    }

    void removeGrid(const IGrid *grid)
    {
        IGrid *old = 0;
        int row = 0;
        foreach (IGrid *g, mGrids) {
            if (g == grid)
                break;
            row++;
        }
        if (!old)
            return;
        beginRemoveRows(QModelIndex(), row, row);
        mGrids.removeAt(row);
        endRemoveRows();
        delete old;
    }

signals:

public slots:

private:
    QList<IGrid *> mGrids;
};


#endif // GRIDMANAGERDIALOG_P_H
