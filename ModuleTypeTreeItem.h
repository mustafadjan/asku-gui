#pragma once

#include "AbstractTreeItem.h"

class ModuleTypeTreeItemPrivate;
enum class ModuleType;

class ModuleTypeTreeItem : public AbstractTreeItem
{
    Q_DECLARE_PRIVATE_D(d, ModuleTypeTreeItem)

public:

    explicit ModuleTypeTreeItem(ModuleType);

    int columnCount() const override;
    QVariant data(int) const override;
    QVector<int> setData(const QVariant&) override;

};
