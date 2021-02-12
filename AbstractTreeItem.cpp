#include "AbstractTreeItem.h"
#include "AbstractTreeItem_p.h"

AbstractTreeItem::AbstractTreeItem(AbstractTreeItemPrivate& d):
    d(&d)
{
    if (this->d->parentItem) {
        this->d->parentItem->d->childItems.append(this);
    }
}

AbstractTreeItem::~AbstractTreeItem()
{
    for (const auto item : d->childItems) {
        delete item;
    }
    if (d->parentItem) { // на случай удаления не через родителя
        d->parentItem->d->childItems.removeOne(this);
    }
    delete d;
}

AbstractTreeItem* AbstractTreeItem::child(int row) const
{
    return d->childItems.value(row, nullptr);
}

bool AbstractTreeItem::hasChild() const
{
    return !d->childItems.isEmpty();
}

int AbstractTreeItem::childCount() const
{
    return d->childItems.size();
}

int AbstractTreeItem::row() const
{
    if (d->parentItem) {
        return d->parentItem->d->childItems.indexOf(const_cast<AbstractTreeItem*>(this));
    }

    return -1;
}

AbstractTreeItem* AbstractTreeItem::parentItem() const
{
    return d->parentItem;
}

AbstractTreeItem* AbstractTreeItem::rootItem() const
{
    auto rootItem = d->parentItem;
    while (rootItem->parentItem()) {
        rootItem = rootItem->parentItem();
    }
    return rootItem;
}
