#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include "VoiTypes.h"

class AbstractElemTreeItem;

class AbstractElemTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(AbstractElemTreeItemPrivate)

public:

    ElemData data;

    explicit AbstractElemTreeItemPrivate(ItemType, quint32, const QString&, AbstractElemTreeItem*);

    bool hasChild(ItemType, bool = false) const;

};
