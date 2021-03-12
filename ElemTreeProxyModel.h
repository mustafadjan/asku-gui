#pragma once

#include "SchemeBaseProxyModel.h"

class ElemTreeProxyModel : public SchemeBaseProxyModel
{
    Q_OBJECT

public:

    explicit ElemTreeProxyModel(QAbstractItemModel*, QObject* = nullptr);

    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override;

};
