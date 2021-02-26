#include "AbstractSchemeTreeItem.h"
#include "AbstractSchemeTreeItem_p.h"
#include <QVariant>

AbstractSchemeTreeItem::AbstractSchemeTreeItem(AbstractSchemeTreeItemPrivate& d):
    d(&d)
{
    if (this->d->parentItem) {
        this->d->parentItem->d->childItems.append(this);
    }
}

AbstractSchemeTreeItem::~AbstractSchemeTreeItem()
{
    removeChilds();
    if (d->parentItem) { // на случай удаления не через родителя
        d->parentItem->d->childItems.removeOne(this);
    }
    delete d;
}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::child(int row) const
{
    return d_func()->childItems.value(row, nullptr);
}

bool AbstractSchemeTreeItem::hasChild() const
{
    return !d_func()->childItems.isEmpty();
}

int AbstractSchemeTreeItem::childCount() const
{
    return d_func()->childItems.size();
}

int AbstractSchemeTreeItem::row() const
{
    if (d->parentItem) {
        return d->parentItem->d->childItems.indexOf(const_cast<AbstractSchemeTreeItem*>(this));
    }

    return -1;
}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::parentItem() const
{
    return d_func()->parentItem;
}

AbstractSchemeTreeItem* AbstractSchemeTreeItem::rootItem() const
{
    auto rootItem = d_func()->parentItem;
    while (rootItem && rootItem->parentItem()) {
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

void AbstractSchemeTreeItem::setName(const QString& name)
{
    d_func()->name = name;
}

void AbstractSchemeTreeItem::removeChilds()
{
    //qDeleteAll(d->childItems);
    for (auto item : d->childItems) {
        delete item;
    }
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
