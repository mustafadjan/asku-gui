#pragma once

#include "ModuleTreeItem_p.h"

class RLKTreeItemPrivate : public ModuleTreeItemPrivate
{
    Q_DISABLE_COPY(RLKTreeItemPrivate)

public:

    quint8 mobility:4;
    quint8 prodType:4;

    explicit RLKTreeItemPrivate(quint32 id):
        ModuleTreeItemPrivate(id, nullptr)
    {
    }

};
