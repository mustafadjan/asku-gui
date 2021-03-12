#include "TuneParamTreeProxyModel.h"
#include "TuneParamTreeItem.h"
#include "VoiTypes.h"

TuneParamTreeProxyModel::TuneParamTreeProxyModel(QAbstractItemModel* sourceModel, QObject* parent):
    SchemeBaseProxyModel(ModelType::TuneParamTree, sourceModel, parent)
{
}

int TuneParamTreeProxyModel::columnCount(const QModelIndex&) const
{
    return 3;
}

QVariant TuneParamTreeProxyModel::data(const QModelIndex& index, int role) const
{
    auto sourceIndex = mapToSource(index);

    if (sourceIndex.isValid()) {
        auto item = static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer());

        int ty = item->data(sourceIndex.column()).userType();
        auto name = item->data(0).toString();
        switch (role) {
            case Qt::DisplayRole:
                return item->data(sourceIndex.column()).userType() == QMetaType::Bool ?
                           QVariant() : item->data(sourceIndex.column());
            case Qt::ChangeTPRole:
                if (sourceIndex.column() == 0) {
                    return item->roleData(role);
                }
                break;
            case Qt::CheckStateRole:
                if (item->data(sourceIndex.column()).userType() == QMetaType::Bool) {
                    bool b = item->data(sourceIndex.column()).toBool();
                    return item->data(sourceIndex.column()).toBool() ?
                               Qt::Checked : Qt::Unchecked;
                }
        }
    }

    return SchemeBaseProxyModel::data(index, role);
}

bool TuneParamTreeProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto sourceIndex = mapToSource(index);

    QVector<int> result;

    if (sourceIndex.isValid()) {
        auto item = static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer());

        if (role == Qt::CheckStateRole && value.canConvert(QVariant::Bool)) {
            result = item->setData(value.toBool());
        }
        else if (role == Qt::EditRole) {
            result = item->setData(value);
        }
        else if (role == Qt::ResetTPRole) {
            //result = item->setData();
        }

        if (!result.isEmpty()) {
            emit dataChanged(index, index, {role});
            auto firstIndex = index.siblingAtColumn(0);
            emit dataChanged(firstIndex, firstIndex, result);
        }
    }

    return !result.isEmpty();
}

QVariant TuneParamTreeProxyModel::headerData(int section, Qt::Orientation orientation, int role)
const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("НП");
            case 1:
                return tr("Текущее значение");
            case 2:
                return tr("Устанавливаемое значение");
        }
    }

    return SchemeBaseProxyModel::headerData(section, orientation, role);
}

Qt::ItemFlags TuneParamTreeProxyModel::flags(const QModelIndex& index) const
{
    auto flags = SchemeBaseProxyModel::flags(index);

    auto sourceIndex = mapToSource(index);

    if (sourceIndex.isValid() && sourceIndex.column() == 2) {
        return flags | (static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer())->
                            data(sourceIndex.column()).type() == QVariant::Bool ?
                                 Qt::ItemIsUserCheckable : Qt::ItemIsEditable);
    }

    return flags;
}

QList<OneConfigParamValue> TuneParamTreeProxyModel::getNewValues(const QModelIndex& index) const
{
    QList<OneConfigParamValue> data;

    int size = rowCount(index);
    for (int i = 0; i < size; ++i) {
        auto childIndex = this->index(i, 0, index);
        if (hasChildren(childIndex)) { // группа НП
            data.append(getNewValues(childIndex));
        }
        else {
            auto item = static_cast<AbstractSchemeTreeItem*>(mapToSource(childIndex).
                                                             internalPointer());
            auto tuneParamItem = dynamic_cast<TuneParamTreeItem*>(item);
            if (tuneParamItem && tuneParamItem->isNewValue()) {
                data.append(tuneParamItem->packaging());
            }
        }
    }

    return data;
}
