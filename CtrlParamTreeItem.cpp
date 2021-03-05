#include "CtrlParamTreeItem.h"
#include "CtrlParamTreeItem_p.h"
#include <QDateTime>

CtrlParamTreeItem::CtrlParamTreeItem(quint32 id, const QString& name,
                                     AbstractSchemeTreeItem* parent):
    AbstractSchemeTreeItem(*new CtrlParamTreeItemPrivate(id, name, parent))
{
    d_func()->itemType = ItemType::CtrlParam;
}

int CtrlParamTreeItem::columnCount() const
{
    return 5;
}

QVariant CtrlParamTreeItem::data(int column) const
{
    switch (column) {
        case 0:
            return d_func()->name;
        case 1:
            return d_func()->data.value;
        case 2:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeRecv).toUTC()
                                                  .toString("hh:mm:ss");
        case 3:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeCond).toUTC()
                                                  .toString("hh:mm:ss");
        case 4:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeVal).toUTC()
                                                  .toString("hh:mm:ss");
    }

    return QVariant();
}

QVariant CtrlParamTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::ConditionRole:
            return conditionBrush();
    }

    return AbstractSchemeTreeItem::roleData(role);
}

QVector<int> CtrlParamTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;

    if (data.canConvert<CtrlParamData>()) {
        d_func()->data = data.value<CtrlParamData>();
        roles << Qt::ConditionRole;
    }

    return roles;
}

bool CtrlParamTreeItem::isValid(ModelType modelType) const
{
    switch (modelType) {
        case ModelType::ElemTree:
            return hasChild();
        case ModelType::CtrlParamTable:
            return true;
        default:
            return false;
    }
}

void CtrlParamTreeItem::setDescription(const QString& description)
{
    d_func()->description = description;
}

void CtrlParamTreeItem::setFormat(const QString& format)
{
    d_func()->format = format;
}

void CtrlParamTreeItem::setTemplates(const QHash<int, QString> templates)
{
    d_func()->templates = templates;
}

AbstractConditionalItem::ItemCondition CtrlParamTreeItem::condition() const
{
    return static_cast<ItemCondition>(d_func()->data.condition);
}

CtrlParamTreeItemPrivate::CtrlParamTreeItemPrivate(quint32 id, const QString& name,
                                                   AbstractSchemeTreeItem* parent):
    AbstractSchemeTreeItemPrivate(id, name, parent)
{
    data.condition = static_cast<quint8>(AbstractConditionalItem::ItemCondition::Norm);
}
