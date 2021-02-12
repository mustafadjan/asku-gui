#include "CtrlParamTableProxyModel.h"
#include "AbstractSchemeTreeItem.h"

CtrlParamTableProxyModel::CtrlParamTableProxyModel(QAbstractItemModel* sourceModel,
                                                   QObject* parent):
    SchemeBaseProxyModel(ModelType::CtrlParamTable, sourceModel, parent)
{
}

int CtrlParamTableProxyModel::columnCount(const QModelIndex&) const
{
    return 4;
}

QVariant CtrlParamTableProxyModel::data(const QModelIndex& index, int role) const
{
    auto sourceIndex = mapToSource(index);

    if (sourceIndex.isValid() && role == Qt::ConditionRole) {
        return static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer())->roleData(role);
    }

    return SchemeBaseProxyModel::data(index, role);
}

QVariant CtrlParamTableProxyModel::headerData(int section, Qt::Orientation orientation, int role)
const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Название");
            case 1:
                return tr("Значение");
            case 2:
                return tr("Время 1");
            case 3:
                return tr("Время 2");
        }
    }

    return SchemeBaseProxyModel::headerData(section, orientation, role);
}
