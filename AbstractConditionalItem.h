#pragma once

#include <QMap>
#include <QBrush>

struct AbstractConditionalItem
{
    enum class ItemCondition : quint8
    {
        Unknown = 0,
        Norm = 1,
        Warn = 2,
        Fail = 3,
        Uncontrol = 4
    };

    virtual ~AbstractConditionalItem() = default;

protected:

    virtual ItemCondition condition() const = 0;
    QBrush conditionBrush() const;

};
