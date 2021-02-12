#include "RlkTreeItem.h"
#include "RlkTreeItem_p.h"

RLKTreeItem::RLKTreeItem(quint32 id, const QString& name):
    ModuleTreeItem(*new RLKTreeItemPrivate(ItemType::RLK, id, name))
{
    if (name.isEmpty()) {
        d_func()->name = QObject::tr("РЛК (%1)").arg(id);
    }
}

bool RLKTreeItem::isValid(ModelType modelType) const
{
    switch (modelType) {
        case ModelType::ElemList:
            return d_func()->hasChild(ItemType::Module);
        default:
            return ModuleTreeItem::isValid(modelType);
    }
}

ModuleTreeItemPrivate::ModuleTreeItemPrivate(ItemType itemType, quint32 id, const QString& name,
                                             RLKTreeItem* parent):
    AbstractElemTreeItemPrivate(itemType, id, name, parent)
{
}
