#include "AbstractConditionalItem.h"
#include <QMap>
#include <QBrush>

static const QMap<EModulState, QBrush> conditionBrushes
{
    {eStUnknown,   QBrush(Qt::lightGray)},
    {eStNorma,     QBrush(Qt::transparent)},
    {eStWarn,      QBrush(Qt::yellow)},
    {eStError,     QBrush(Qt::red)},
    {eStNoControl, QBrush(Qt::darkGray)},
};

QBrush AbstractConditionalItem::conditionBrush() const
{
    return conditionBrushes[condition()];
}
