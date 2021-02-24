#include "RlkTreeItem.h"
#include "RlkTreeItem_p.h"

RLKTreeItem::RLKTreeItem(quint32 id):
    ModuleTreeItem(*new RLKTreeItemPrivate(id))
{
    d_func()->itemType = ItemType::RLK;

    setName();
}

void RLKTreeItem::setName(const QString& name)
{
    if (name.isEmpty()) {
        d_func()->name = QObject::tr("РЛК (%1)").arg(d_func()->id);
    }
    else {
        d_func()->name = name;
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

QVector<int> RLKTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;

    if (data.canConvert<Pack0x24_D002>()) {
        auto castedData = data.value<Pack0x24_D002>();
        roles = ModuleTreeItem::setData(QVariant::fromValue(static_cast<Pack0x24>(castedData)));
        d_func()->mobility = castedData.type;
        d_func()->prodType = castedData.prodName;
        // todo: дозаполнить вектор ролей, если понадобится
    }

    return roles;
}

ModuleTreeItemPrivate::ModuleTreeItemPrivate(quint32 id, RLKTreeItem* parent):
    AbstractElemTreeItemPrivate(id, {}, parent)
{
}
