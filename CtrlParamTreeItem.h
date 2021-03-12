#pragma once

#include "AbstractSchemeTreeItem.h"
#include "AbstractConditionalItem.h"
#include "AbstractValuableItem.h"

class CtrlParamTreeItemPrivate;

class CtrlParamTreeItem : public AbstractSchemeTreeItem, public AbstractConditionalItem,
                          public AbstractValuableItem
{
    Q_DECLARE_PRIVATE_D(d, CtrlParamTreeItem)

public:

    explicit CtrlParamTreeItem(quint32, const QString&, AbstractSchemeTreeItem*);

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override; 
    bool isValid(ModelType) const override;

    void setDescription(const QString&);
    void setFormat(const QString&);
    void setTemplates(const QHash<int, QString>);

protected:

    EModulState condition() const override;
    void setValueType(QMetaType::Type) override;

};
