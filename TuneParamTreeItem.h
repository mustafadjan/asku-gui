#pragma once

#include "AbstractSchemeTreeItem.h"
#include "AbstractValuableItem.h"

class TuneParamTreeItemPrivate;
class OneConfigParamValue;

class TuneParamTreeItem : public AbstractSchemeTreeItem, public AbstractValuableItem
{
    Q_DECLARE_PRIVATE_D(d, TuneParamTreeItem)

public:

    explicit TuneParamTreeItem(quint32, const QString&, AbstractSchemeTreeItem*);

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override;
    bool isValid(ModelType) const override;

    void setUnit(const QString&);
    void setSaveProp(bool);
    void setMin(float);
    void setMax(float);
    void setValues(const QStringList&);
    void setDescription(const QString&);
    //void setUserClearance(some enum class or int);

    bool isNewValue() const;
    OneConfigParamValue packaging() const;
    QWidget* createEditor(QWidget*) const;
    QVariant editorData(QWidget*) const;
    void resetOuterData();
    //void updateOuterData();

protected:

    void setValueType(QMetaType::Type) override;

};
