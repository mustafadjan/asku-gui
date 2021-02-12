#pragma once

#include <QTreeView>

class TuneParamTreeView : public QTreeView
{
    Q_OBJECT

public:

    explicit TuneParamTreeView(QAbstractItemModel*, QWidget* = nullptr);

public slots:

    void setRootIndex(const QModelIndex&) override;
    void sendChangedTP();

private slots:

    void contextMenu(const QPoint&);
};
