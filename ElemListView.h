#pragma once

#include <QListView>

class ElemListView : public QListView
{
    Q_OBJECT

public:

    explicit ElemListView(QAbstractItemModel*, QWidget* = nullptr);

public slots:

    void setRootIndex(const QModelIndex&) override;

};
