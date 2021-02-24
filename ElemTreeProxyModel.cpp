#include "ElemTreeProxyModel.h"
#include "AbstractSchemeTreeItem.h"

ElemTreeProxyModel::ElemTreeProxyModel(QAbstractItemModel* sourceModel, QObject* parent):
    SchemeBaseProxyModel(ModelType::ElemTree, sourceModel, parent)
{
}

ElemTreeProxyModel::~ElemTreeProxyModel()
{
}

QVariant ElemTreeProxyModel::data(const QModelIndex& index, int role) const
{
    auto sourceIndex = mapToSource(index);

    if (sourceIndex.isValid() && (role == Qt::ConditionRole || role == Qt::StateRole)) {
        return static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer())->roleData(role);
    }

    return SchemeBaseProxyModel::data(index, role);
}

QVariant ElemTreeProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Название");
        }
    }

    return SchemeBaseProxyModel::headerData(section, orientation, role);
}
