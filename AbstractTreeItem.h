#pragma once

#include <QVariant>

class AbstractTreeItemPrivate;

class AbstractTreeItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractTreeItem)

public:

    AbstractTreeItem* child(int) const;
    bool hasChild() const;
    int childCount() const;
    int row() const;
    AbstractTreeItem* parentItem() const;
    AbstractTreeItem* rootItem() const;

    virtual int columnCount() const = 0;
    virtual QVariant data(int) const = 0;
    virtual QVector<int> setData(const QVariant&) = 0;

protected:

    explicit AbstractTreeItem(AbstractTreeItemPrivate&);
    virtual ~AbstractTreeItem();

    AbstractTreeItemPrivate* const d;

};
