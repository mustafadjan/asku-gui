#pragma once

#include "SchemeBaseProxyModel.h"

class CtrlParamTableProxyModel : public SchemeBaseProxyModel
{
    Q_OBJECT

public:

    explicit CtrlParamTableProxyModel(QAbstractItemModel*, QObject* = nullptr);

    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
    // заголовок столбца
    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override;

};
