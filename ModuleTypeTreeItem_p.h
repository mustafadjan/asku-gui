#pragma once

#include "AbstractTreeItem_p.h"

enum class ModuleType;

class ModuleTypeTreeItemPrivate : public AbstractTreeItemPrivate
{
    Q_DISABLE_COPY(ModuleTypeTreeItemPrivate)

public:

    ModuleType type;

    explicit ModuleTypeTreeItemPrivate(ModuleType);

};
