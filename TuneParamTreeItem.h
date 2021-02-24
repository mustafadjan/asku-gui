#pragma once

#include "AbstractSchemeTreeItem.h"
#include <QMetaType>

class TuneParamTreeItemPrivate;

class TuneParamTreeItem : public AbstractSchemeTreeItem
{
    Q_DECLARE_PRIVATE_D(d, TuneParamTreeItem)

public:

    explicit TuneParamTreeItem(quint32, const QString&, AbstractSchemeTreeItem*);
    explicit TuneParamTreeItem(const QString&, AbstractSchemeTreeItem*);

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override;
    bool isValid(ModelType) const override;

    void setType(QMetaType::Type);
    void setUnit(const QString&);
    void setSaveStatus(bool);
    void setMin(float);
    void setMax(float);
    void setValues(const QVector<QString>&);
    void setDescription(const QString&);
    void setDefault(const QVariant&);
    //void setUserClearance(some enum class or int);

    QWidget* createEditor(QWidget*) const;
    QVariant editorData(QWidget*) const;
    void resetOuterData();
    //void updateOuterData();

};
