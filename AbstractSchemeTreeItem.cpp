#include "AbstractSchemeTreeItem.h"
#include "AbstractSchemeTreeItem_p.h"

AbstractSchemeTreeItem::AbstractSchemeTreeItem(AbstractSchemeTreeItemPrivate& d):
    AbstractTreeItem(d)
{
}

//AbstractSchemeTreeItem::~AbstractSchemeTreeItem()
//{
//    for (const auto item : d->childItems) {
//        delete item;
//    }
//    if (d->parentItem) { // на случай удаления не через родителя
//        d->parentItem->d->childItems.removeOne(this);
//    }
//    delete d;
//}
//
//AbstractSchemeTreeItem* AbstractSchemeTreeItem::child(int row) const
//{
//    return d->childItems.value(row);
//}
//
//bool AbstractSchemeTreeItem::hasChild() const
//{
//    return !d->childItems.isEmpty();
//}
//
//int AbstractSchemeTreeItem::childCount() const
//{
//    return d->childItems.size();
//}
//
//int AbstractSchemeTreeItem::row() const
//{
//    if (d->parentItem) {
//        return d->parentItem->d->childItems.indexOf(const_cast<AbstractSchemeTreeItem*>(this));
//    }
//
//    return -1;
//}
//
//AbstractSchemeTreeItem* AbstractSchemeTreeItem::parentItem() const
//{
//    return d->parentItem;
//}
//
//AbstractSchemeTreeItem* AbstractSchemeTreeItem::rootItem() const
//{
//    auto rootItem = d->parentItem;
//    while (rootItem->parentItem()) {
//        rootItem = rootItem->parentItem();
//    }
//    return rootItem;
//}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::child(int row) const
{
    return dynamic_cast<AbstractSchemeTreeItem*>(AbstractTreeItem::child(row));
}

int AbstractSchemeTreeItem::row() const
{
    if (parentItem()) {
        return parentItem()->d->childItems.indexOf(const_cast<AbstractSchemeTreeItem*>(this));
    }

    return -1;
}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::parentItem() const
{
    return dynamic_cast<AbstractSchemeTreeItem*>(AbstractTreeItem::parentItem());
}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::rootItem() const
{
    auto rootItem = parentItem();
    while (rootItem) {
        rootItem = rootItem->parentItem();
    }
    return rootItem;
}

ItemType AbstractSchemeTreeItem::type() const
{
    return d_func()->itemType;
}

quint32 AbstractSchemeTreeItem::id() const
{
    return d_func()->id;
}

QVariant AbstractSchemeTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::FullPathRole:
        {
            QStringList fullPath;

            for (auto item = this; item;
                 item = dynamic_cast<AbstractSchemeTreeItem*>(item->parentItem())) {
                fullPath.prepend(item->d_func()->name);
            }

            return fullPath.join('/');
        }
        case Qt::IDRole:
            return id();
    }

    return QVariant();
}
