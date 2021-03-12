#pragma once

#include <QTreeView>

struct OneConfigParamValue;

class TuneParamTreeView : public QTreeView
{
    Q_OBJECT

public:

    explicit TuneParamTreeView(QAbstractItemModel*, QWidget* = nullptr);

public slots:

    void setRootIndex(const QModelIndex&) override;
    void sendNewValues();
    void sendNewValuesSaveProp();

private slots:

    void contextMenu(const QPoint&);

signals:

    void newValues(quint32, quint32, qint64, const QList<OneConfigParamValue>&);

};
