#pragma once

#include "ModuleTreeItem.h"

class RLKTreeItemPrivate;

class RLKTreeItem : public ModuleTreeItem
{
    Q_DECLARE_PRIVATE_D(d, RLKTreeItem)

public:

    explicit RLKTreeItem(quint32);

    void setName(const QString& = QString());
    bool isValid(ModelType) const override;
    QVector<int> setData(const QVariant&) override;

};
