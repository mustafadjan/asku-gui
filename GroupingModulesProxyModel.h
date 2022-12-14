#pragma once

#include <QAbstractProxyModel>

class GroupingModulesProxyModelPrivate;

class GroupingModulesProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE_D(d, GroupingModulesProxyModel)
    Q_DISABLE_COPY(GroupingModulesProxyModel)

public:

    explicit GroupingModulesProxyModel(QAbstractItemModel*, QObject* = nullptr);
    ~GroupingModulesProxyModel() override;

    // следующие два метода проксируют и депроксируют индексы
    QModelIndex mapToSource(const QModelIndex&) const override; // прокси в исходный
    QModelIndex mapFromSource(const QModelIndex&) const override; // исходный в прокси
    QModelIndex index(int, int, const QModelIndex& = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:

    GroupingModulesProxyModelPrivate* const d;

};
