#pragma once

#include <QWidget>

class QAbstractItemModel;
struct OneConfigParamValue;

class TuneParamWidget : public QWidget
{
    Q_OBJECT

public:

    explicit TuneParamWidget(QAbstractItemModel*, QWidget* = nullptr);

signals:

    void currentChanged(const QModelIndex&);
    void newValues(quint32, quint32, qint64, const QList<OneConfigParamValue>&);

};
