#pragma once

#include "SchemeBaseProxyModel.h"

struct OneConfigParamValue;

class TuneParamTreeProxyModel : public SchemeBaseProxyModel
{
    Q_OBJECT

public:

    explicit TuneParamTreeProxyModel(QAbstractItemModel*, QObject* = nullptr);

    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int = Qt::EditRole) override;
    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex&) const override;

    QList<OneConfigParamValue> getNewValues(const QModelIndex&) const;

};
