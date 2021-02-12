#pragma once

#include <QWidget>

class QAbstractItemModel;

class TuneParamWidget : public QWidget
{
    Q_OBJECT

public:

    explicit TuneParamWidget(QAbstractItemModel*, QWidget* = nullptr);

signals:

    void currentChanged(const QModelIndex&);

};
