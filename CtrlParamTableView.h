#pragma once

#include <QTableView>

class CtrlParamTableView : public QTableView
{
    Q_OBJECT

public:

    explicit CtrlParamTableView(QAbstractItemModel*, QWidget* = nullptr);

public slots:

    void setRootIndex(const QModelIndex&) override;

protected:

    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>& = {}) override;

};
