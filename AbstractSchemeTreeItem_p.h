#pragma once

#include "AbstractTreeItem_p.h"

enum class ItemType : quint8;
//class AbstractSchemeTreeItem;

class AbstractSchemeTreeItemPrivate : public AbstractTreeItemPrivate
{
    Q_DISABLE_COPY(AbstractSchemeTreeItemPrivate)

public:

    ItemType itemType;
    quint32 id{0};
    qint64 time{0};
    //QString name;
    //
    //QVector<AbstractSchemeTreeItem*> childItems;
    //AbstractSchemeTreeItem* parentItem{nullptr};

protected:

    explicit AbstractSchemeTreeItemPrivate(ItemType itemType, quint32 id, const QString& name,
                                           AbstractTreeItem* parent = nullptr):
        AbstractTreeItemPrivate(name, parent),
        itemType(itemType),
        id(id)
        //name(name),
        //parentItem(parent)
    {
    }

};
