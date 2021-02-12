#include "ElemTreeItem.h"
#include "AbstractElemTreeItem_p.h"

ElemTreeItem::ElemTreeItem(quint16 id, const QString& data, AbstractElemTreeItem* parent):
    AbstractElemTreeItem(*new AbstractElemTreeItemPrivate(ItemType::Elem, id, data, parent))
{
}

QVector<int> ElemTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;

    if (data.canConvert<ElemData>()) {
        d_func()->data = data.value<ElemData>();
        roles << Qt::ConditionRole << Qt::StateRole;
    }

    return roles;
}
