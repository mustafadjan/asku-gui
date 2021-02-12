#pragma once

#include "AbstractElemTreeItem.h"

class RLKTreeItem;
class ModuleTreeItemPrivate;

class ModuleTreeItem : public AbstractElemTreeItem
{
    Q_DECLARE_PRIVATE_D(d, ModuleTreeItem)

public:

    explicit ModuleTreeItem(quint32, const QString&, RLKTreeItem*);

    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override;

protected:

    explicit ModuleTreeItem(ModuleTreeItemPrivate&);

};
