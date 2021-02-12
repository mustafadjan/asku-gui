#pragma once

#include "AbstractSchemeTreeItem.h"

class TuneParamTreeItemPrivate;

class TuneParamTreeItem : public AbstractSchemeTreeItem
{
    Q_DECLARE_PRIVATE_D(d, TuneParamTreeItem)

public:

    explicit TuneParamTreeItem(quint32, const QString&, AbstractSchemeTreeItem*);
    explicit TuneParamTreeItem(quint32, const QString&, const QVariant&, AbstractSchemeTreeItem*);
    explicit TuneParamTreeItem(quint32, const QString&, const QVariant&,
                               const QPair<float, float>&, AbstractSchemeTreeItem*);
    explicit TuneParamTreeItem(quint32, const QString&, const QString&, const QSet<QString>&,
                               AbstractSchemeTreeItem*);

    int columnCount() const override;
    QVariant data(int) const override;
    QVariant roleData(int) const override;
    QVector<int> setData(const QVariant&) override;
    bool isValid(ModelType) const override;

    QWidget* createEditor(QWidget*) const;
    QVariant editorData(QWidget*) const;
    void resetOuterData();
    void updateOuterData();

};
