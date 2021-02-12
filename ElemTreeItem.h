#pragma once

#include "AbstractElemTreeItem.h"

class ElemTreeItem : public AbstractElemTreeItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractElemTreeItem)

public:

    explicit ElemTreeItem(quint16, const QString&, AbstractElemTreeItem*);

    QVector<int> setData(const QVariant&) override;

};
