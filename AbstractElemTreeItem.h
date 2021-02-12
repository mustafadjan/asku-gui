#pragma once

#include "AbstractSchemeTreeItem.h"
#include "AbstractConditionalItem.h"

class AbstractElemTreeItemPrivate;

class AbstractElemTreeItem : public AbstractSchemeTreeItem, public AbstractConditionalItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractElemTreeItem)

public:

    enum class State : quint8
    {
        On = 0,
        Off = 1,
        TurnOn = 2,
        TurnOff = 3
    };

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    bool isValid(ModelType) const override;

protected:

    explicit AbstractElemTreeItem(AbstractElemTreeItemPrivate&);

    ItemCondition condition() const override;

};
