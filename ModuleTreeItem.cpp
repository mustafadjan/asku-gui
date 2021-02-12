#include "ModuleTreeItem.h"
#include "ModuleTreeItem_p.h"

ModuleTreeItem::ModuleTreeItem(quint32 id, const QString& name, RLKTreeItem* parent):
    AbstractElemTreeItem(*new ModuleTreeItemPrivate(ItemType::Module, id, name, parent))
{
}

ModuleTreeItem::ModuleTreeItem(ModuleTreeItemPrivate& d):
    AbstractElemTreeItem(d)
{
}

QVariant ModuleTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::IsModuleRole:
            return d_func()->itemType == ItemType::Module;
    }

    return AbstractElemTreeItem::roleData(role);
}

QVector<int> ModuleTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;

    if (data.canConvert<Pack0x24>()) {
        auto castedData = data.value<Pack0x24>();
        d_func()->data.time = castedData.time;
        d_func()->data.condition = castedData.condition;
        d_func()->data.state = castedData.state;
        d_func()->data.local = castedData.local;
        d_func()->data.imit = castedData.imit;
        d_func()->RGDV = castedData.RGDV;
        d_func()->RAF = castedData.RAF;
        d_func()->hasTP = castedData.hasTP;
        d_func()->TP = castedData.TP;
        d_func()->mode = castedData.mode;
        d_func()->numVOI = static_cast<quint16>(castedData.numVOI);
        roles << Qt::ConditionRole << Qt::StateRole;
        // todo: дозаполнить вектор ролей, если понадобится
    }

    return roles;
}
