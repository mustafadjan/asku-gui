#pragma once

#include "AbstractElemTreeItem.h"
#include <QString>

class ModuleTreeItemPrivate;
class RLKTreeItem;

class ModuleTreeItem : public AbstractElemTreeItem
{
    Q_DECLARE_PRIVATE_D(d, ModuleTreeItem)

public:

    explicit ModuleTreeItem(quint32, RLKTreeItem*);

    QVariant roleData(int) const override;
    void setName(const QString& = QString());
    QVector<int> setData(const QVariant&) override;

protected:

    explicit ModuleTreeItem(ModuleTreeItemPrivate&);

};
