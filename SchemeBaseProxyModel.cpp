#include "SchemeBaseProxyModel.h"
#include "AbstractSchemeTreeItem.h"

SchemeBaseProxyModel::SchemeBaseProxyModel(ModelType modelType, QAbstractItemModel* sourceModel,
                                           QObject* parent):
    QSortFilterProxyModel(parent),
    modelType(modelType)
{
    setSourceModel(sourceModel);
}

QVector<int> SchemeBaseProxyModel::rowsToHide(QList<ItemType> itemTypes, const QModelIndex& parent,
                                              bool (*cond)(AbstractSchemeTreeItem*)) const
{
    QVector<int> rows;

    int row = rowCount(parent);
    for (int i = 0; i < row; ++i) {
        auto sourceIndex = mapToSource(index(i, 0, parent));
        if (sourceIndex.isValid()) {
            auto item = static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer());
            if (!itemTypes.contains(item->type()) || cond(item)) {
                rows << i;
            }
        }
    }

    return rows;
}

bool SchemeBaseProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto item = static_cast<AbstractSchemeTreeItem*>(index.internalPointer());
    // узел сообщает подходит ли он для конкретного проксирования
    return item->isValid(modelType);
}
