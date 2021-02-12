#pragma once

#include "AbstractSchemeTreeItem.h"
#include "AbstractConditionalItem.h"

class CtrlParamTreeItemPrivate;

class CtrlParamTreeItem : public AbstractSchemeTreeItem, public AbstractConditionalItem
{
    Q_DECLARE_PRIVATE_D(d, CtrlParamTreeItem)

public:

    explicit CtrlParamTreeItem(quint32, const QString&, AbstractSchemeTreeItem*);

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override; 
    bool isValid(ModelType) const override;

protected:

    ItemCondition condition() const override;

};
