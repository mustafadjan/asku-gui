#include "ElemListProxyModel.h"
#include "AbstractSchemeTreeItem.h"

ElemListProxyModel::ElemListProxyModel(QAbstractItemModel* sourceModel, QObject* parent):
    SchemeBaseProxyModel(ModelType::ElemList, sourceModel, parent)
{
}

QVariant ElemListProxyModel::data(const QModelIndex& index, int role) const
{
    auto sourceIndex = mapToSource(index);

    if (sourceIndex.isValid()) {
        switch (role) {
            case Qt::DisplayRole:
                return QVariant();
            case Qt::StateRole:
                return static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer())->
                           roleData(Qt::ElemFlagsRole);
        }
    }

    return SchemeBaseProxyModel::data(index, role);
}
