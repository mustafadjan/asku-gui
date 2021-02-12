#pragma once

#include "SchemeBaseProxyModel.h"

class ElemListProxyModel : public SchemeBaseProxyModel
{
    Q_OBJECT

public:

    explicit ElemListProxyModel(QAbstractItemModel*, QObject* = nullptr);

    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;

};
