#include "GroupingModulesProxyModel.h"
#include "AbstractSchemeTreeItem.h"
#include "VoiTypes.h"
#include "Enums.h"
#include "TypesFunction.h"
#include <QDebug>

//struct Mapping;
//typedef QHash<QModelIndex, Mapping*>::const_iterator MappingIt;
//typedef QMap<ModuleType, QVector<int>>::const_iterator SourceRowsIt;

class GroupingModulesProxyModelPrivate
{
    Q_DECLARE_PUBLIC(GroupingModulesProxyModel)

    GroupingModulesProxyModel* q_ptr{nullptr};

    struct Mapping;
    typedef QHash<QModelIndex, Mapping*>::const_iterator MappingIt;
    typedef QMap<EIdUser, QVector<int>>::const_iterator SourceRowsIt;

    struct Mapping
    {
        MappingIt it;
    };

    QMap<QModelIndex, QMap<EIdUser, QVector<int>>> sourceRows;
    mutable QHash<QModelIndex, Mapping*> mappedIndexes;

    MappingIt createMapping(const QModelIndex&) const;
    MappingIt createMapping(const QModelIndex&, EIdUser) const;
    MappingIt indexToIterator(const QModelIndex&) const;
    SourceRowsIt findSourceRowsIt(const QModelIndex&) const;
    EIdUser findModuleType(const QModelIndex&) const;
    const QVector<int>& findSourceRows(const QModelIndex&) const;
};

GroupingModulesProxyModelPrivate::MappingIt
GroupingModulesProxyModelPrivate::createMapping(const QModelIndex& parent) const
{
    auto it = mappedIndexes.constFind(parent);
    if (it == mappedIndexes.constEnd()) {
        auto mapping = new Mapping();
        mapping->it = it = mappedIndexes.insert(parent, mapping);
    }

    return it;
}

GroupingModulesProxyModelPrivate::MappingIt
GroupingModulesProxyModelPrivate::createMapping(const QModelIndex& proxyParent, EIdUser moduleType)
const
{
    if (proxyParent.isValid()) {
        auto groupedModulesIt = sourceRows.constFind(indexToIterator(proxyParent).key());
        if (groupedModulesIt != sourceRows.constEnd() &&
            groupedModulesIt.value().contains(moduleType)) {
            return createMapping(proxyParent);
        }
    }

    return mappedIndexes.constEnd();
}

GroupingModulesProxyModelPrivate::MappingIt
GroupingModulesProxyModelPrivate::indexToIterator(const QModelIndex& proxyIndex) const
{
    Q_ASSERT_X(proxyIndex.model() == q_func(), __PRETTY_FUNCTION__, "index from wrong model");
    return static_cast<const Mapping*>(proxyIndex.internalPointer())->it;
}

GroupingModulesProxyModelPrivate::SourceRowsIt
GroupingModulesProxyModelPrivate::findSourceRowsIt(const QModelIndex& proxyIndex) const
{
    if (proxyIndex.isValid()) {
        auto index = proxyIndex;
        auto parent = indexToIterator(index).key();
        while (index.isValid() && !sourceRows.contains(parent)) {
            index = parent;
            parent = indexToIterator(index).key();
        }

        auto it = sourceRows.constFind(parent);
        if (it != sourceRows.constEnd() && it.value().size() > index.row()) {
            return it.value().constBegin() += index.row();
        }
    }

    return SourceRowsIt();
}

EIdUser GroupingModulesProxyModelPrivate::findModuleType(const QModelIndex& proxyIndex) const
{
    auto it = findSourceRowsIt(proxyIndex);
    Q_ASSERT_X(it != SourceRowsIt(), __PRETTY_FUNCTION__, "wrong index");
    return it.key();
}

const QVector<int>& GroupingModulesProxyModelPrivate::findSourceRows(const QModelIndex& proxyIndex)
const
{
    auto it = findSourceRowsIt(proxyIndex);
    Q_ASSERT_X(it != SourceRowsIt(), __PRETTY_FUNCTION__, "wrong index");
    return it.value();
}


GroupingModulesProxyModel::GroupingModulesProxyModel(QAbstractItemModel* sourceModel,
                                                     QObject* parent):
    QAbstractProxyModel(parent),
    d(new GroupingModulesProxyModelPrivate())
{
    d_func()->q_ptr = this;

    int rows = sourceModel->rowCount();
    for (int i = 0; i < rows; ++i) {
        auto sourceParent = sourceModel->index(i, 0);
        int rows = sourceModel->rowCount(sourceParent);
        for (int i = 0; i < rows; ++i) {
            auto sourceChild = sourceModel->index(i, 0, sourceParent);
            if (sourceModel->data(sourceChild, Qt::IsModuleRole).toBool()) {
                auto moduleType = static_cast<EIdUser>(sourceModel->
                                                       data(sourceChild, Qt::IDRole).toInt() >> 8);
                d_func()->sourceRows[sourceParent][moduleType].append(sourceChild.row());
            }
        }
    }

    connect(sourceModel, &QAbstractItemModel::dataChanged, this, &QAbstractItemModel::dataChanged);
    //connect(this, &QAbstractItemModel::dataChanged, this, []
    //{
    //    qDebug() << __PRETTY_FUNCTION__;
    //});

    setSourceModel(sourceModel);
}

GroupingModulesProxyModel::~GroupingModulesProxyModel()
{
    qDeleteAll(d_func()->mappedIndexes);
    delete d;
}

QModelIndex GroupingModulesProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
    if (proxyIndex.model() == this) {
        auto it = d_func()->indexToIterator(proxyIndex);
        if (it != d_func()->mappedIndexes.constEnd()) {
            const auto& parent = it.key();
            if (!d_func()->sourceRows.contains(parent)) {
                int sourceRow = -1;
                QModelIndex sourceParent;
                if (parent.model() == this) { // модуль
                    const auto& sourceRows = d_func()->findSourceRows(parent);
                    if (sourceRows.size() > proxyIndex.row()) {
                        sourceRow = sourceRows.at(proxyIndex.row());
                        sourceParent = d_func()->indexToIterator(parent).key();
                    }
                }
                else {
                    sourceRow = proxyIndex.row();
                    sourceParent = parent;
                }
                return sourceModel()->index(sourceRow, proxyIndex.column(), sourceParent);
            }
        }
    }

    return QModelIndex();
}

QModelIndex GroupingModulesProxyModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    //qDebug() << __PRETTY_FUNCTION__ << sourceModel()->data(sourceIndex);
    if (sourceIndex.model() == sourceModel()) {
        int row = -1;
        auto it = d_func()->mappedIndexes.constEnd();
        if (sourceModel()->data(sourceIndex, Qt::IsModuleRole).toBool()) {
            auto sourceParent = sourceIndex.parent();
            auto moduleType = static_cast<EIdUser>(sourceModel()->data(sourceIndex, Qt::IDRole).
                                                   toInt() >> 8);
            int proxyRow = d_func()->sourceRows[sourceParent].keys().indexOf(moduleType);
            auto parentIt = d_func()->createMapping(sourceParent);
            auto proxyIndex = createIndex(proxyRow, 0, *parentIt);

            row = d_func()->findSourceRows(proxyIndex).indexOf(sourceIndex.row());
            it = d_func()->createMapping(proxyIndex, moduleType);
        }
        else {
            row = sourceIndex.row();
            it = d_func()->createMapping(sourceIndex.parent());
        }

        if (row >= 0 && it != d_func()->mappedIndexes.constEnd()) {
            return createIndex(row, sourceIndex.column(), *it);
        }
    }

    return QModelIndex();
}

QModelIndex GroupingModulesProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    //qDebug() << __PRETTY_FUNCTION__ << row << column << parent << hasIndex(row, column, parent);
    if (hasIndex(row, column, parent)) {
        auto it = d_func()->mappedIndexes.constEnd();
        auto sourceParent = mapToSource(parent);
        if (parent.isValid() && !sourceParent.isValid()) {
            it = d_func()->createMapping(parent, d_func()->findModuleType(parent));
        }
        else {
            it = d_func()->createMapping(sourceParent);
        }

        return createIndex(row, column, *it);
    }

    return QModelIndex();
}

QModelIndex GroupingModulesProxyModel::parent(const QModelIndex& child) const
{
    //qDebug() << __PRETTY_FUNCTION__ << child;
    if (child.isValid()) {
        auto it = d_func()->indexToIterator(child);
        if (it != d_func()->mappedIndexes.constEnd()) {
            const auto& parent = it.key();
            return parent.model() == this ? parent : mapFromSource(parent);
        }
    }

    return QModelIndex();
}

int GroupingModulesProxyModel::rowCount(const QModelIndex& parent) const
{
    //qDebug() << __PRETTY_FUNCTION__ << parent;
    if (parent.column() <= 0) {
        auto sourceParent = mapToSource(parent);
        if (d_func()->sourceRows.contains(sourceParent)) {
            return d_func()->sourceRows[sourceParent].size();
        }
        if (parent.isValid() && !sourceParent.isValid()) {
            return d_func()->sourceRows[d_func()->indexToIterator(parent).key()]
                                       [d_func()->findModuleType(parent)].size();
        }
        return sourceModel()->rowCount(sourceParent);
    }

    return 0;
}

int GroupingModulesProxyModel::columnCount(const QModelIndex& parent) const
{
    auto sourceParent = mapToSource(parent);
    if (parent.isValid() == sourceParent.isValid()) {
        return sourceModel()->columnCount(sourceParent);
    }

    return 1;
}

QVariant GroupingModulesProxyModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid()) {
        auto sourceIndex = mapToSource(index);
        if (sourceIndex.isValid()) {
            return sourceModel()->data(sourceIndex, role);
        }
        else if (role == Qt::DisplayRole) {
            return idToStr(d_func()->findModuleType(index)).append(" [%1]").arg(rowCount(index));
        }
    }

    return QVariant();
}

Qt::ItemFlags GroupingModulesProxyModel::flags(const QModelIndex& index) const
{
    auto sourceIndex = mapToSource(index);
    //if (sourceIndex.isValid()) {
        return sourceModel()->flags(sourceIndex);
    //}
    //
    //return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
