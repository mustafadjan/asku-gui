#pragma once

#include "Enums.h"
#include <QMetaType>

struct AbstractValuableItem
{

    virtual ~AbstractValuableItem() = default;

    void setType(ParamType type)
    {
        switch (type) {
            case Float:
                setValueType(QMetaType::Float);
                break;
            case String:
                setValueType(QMetaType::QString);
                break;
            case Int32:
                setValueType(QMetaType::Int);
                break;
            case Bool:
                setValueType(QMetaType::Bool);
                break;
            case Group:
                setValueType(QMetaType::User);
                break;
        }
    }

protected:

    virtual void setValueType(QMetaType::Type) = 0;

};
