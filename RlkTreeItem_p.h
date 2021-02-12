#pragma once

#include "ModuleTreeItem_p.h"

class RLKTreeItemPrivate : public ModuleTreeItemPrivate
{
    Q_DISABLE_COPY(RLKTreeItemPrivate)

public:



    explicit RLKTreeItemPrivate(ItemType itemType, quint32 id, const QString& name):
        ModuleTreeItemPrivate(itemType, id, name, nullptr)
    {
        if (this->name.isEmpty()) {
            this->name = QObject::tr("РЛК (%1)").arg(id);
        }
    }

};
