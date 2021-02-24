#include "ModuleTreeItem.h"
#include "ModuleTreeItem_p.h"
#include "TypesFunction.h"

ModuleTreeItem::ModuleTreeItem(quint32 id, RLKTreeItem* parent):
    AbstractElemTreeItem(*new ModuleTreeItemPrivate(id, parent))
{
    d_func()->itemType = ItemType::Module;

    setName();
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

void ModuleTreeItem::setName(const QString& name)
{
    if (name.isEmpty()) {
        d_func()->name = QString("%1 (%2)").arg(idToStr(d_func()->id >> 8))
                                           .arg(d_func()->id & 0xFF);
    }
    else {
        d_func()->name = name;
    }
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
        roles << Qt::ConditionRole << Qt::StateRole;
        // todo: дозаполнить вектор ролей, если понадобится
    }

    return roles;
}
