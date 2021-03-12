#pragma once

#include "Enums.h"

class QBrush;

struct AbstractConditionalItem
{

    virtual ~AbstractConditionalItem() = default;

protected:

    virtual EModulState condition() const = 0;
    QBrush conditionBrush() const;

};
