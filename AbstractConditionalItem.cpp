#include "AbstractConditionalItem.h"

static const QMap<AbstractConditionalItem::ItemCondition, QBrush> conditionBrushes
{
    {AbstractConditionalItem::ItemCondition::Unknown,   QBrush(Qt::lightGray)},
    {AbstractConditionalItem::ItemCondition::Norm,      QBrush(Qt::transparent)},
    {AbstractConditionalItem::ItemCondition::Warn,      QBrush(Qt::yellow)},
    {AbstractConditionalItem::ItemCondition::Fail,      QBrush(Qt::red)},
    {AbstractConditionalItem::ItemCondition::Uncontrol, QBrush(Qt::darkGray)},
};

QBrush AbstractConditionalItem::conditionBrush() const
{
    return conditionBrushes[condition()];
}
