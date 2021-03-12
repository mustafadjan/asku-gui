#pragma once

#include "AbstractSchemeTreeItem.h"
#include "AbstractConditionalItem.h"

class AbstractElemTreeItemPrivate;

class AbstractElemTreeItem : public AbstractSchemeTreeItem, public AbstractConditionalItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractElemTreeItem)

public:

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    bool isValid(ModelType) const override;

    quint32 rlkId() const { return rootItem()->id(); }
    quint32 moduleId() const { return preRootItem()->id(); }

protected:

    explicit AbstractElemTreeItem(AbstractElemTreeItemPrivate&);

    EModulState condition() const override;

};
