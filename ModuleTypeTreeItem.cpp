#include "ModuleTypeTreeItem.h"
#include "ModuleTypeTreeItem_p.h"
#include "ModuleTreeItem.h"
#include "VoiTypes.h"

ModuleTypeTreeItem::ModuleTypeTreeItem(ModuleType moduleType):
    AbstractTreeItem(*new ModuleTypeTreeItemPrivate(moduleType))
{
}

int ModuleTypeTreeItem::columnCount() const
{
    return 1;
}

QVariant ModuleTypeTreeItem::data(int column) const
{
    switch (column) {
        case 0:
            return d_func()->name;
    }

    return QVariant();
}

QVector<int> ModuleTypeTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;
    return roles;
}

ModuleTypeTreeItemPrivate::ModuleTypeTreeItemPrivate(ModuleType moduleType):
    AbstractTreeItemPrivate(moduleName(moduleType)),
    type(moduleType)
{
}
