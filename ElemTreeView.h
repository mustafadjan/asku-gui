#pragma once

#include <QTreeView>

class ElemTreeView : public QTreeView
{
    Q_OBJECT

public:

    explicit ElemTreeView(QAbstractItemModel*, QWidget* = nullptr);

    QAbstractItemModel* model() const;

protected:

    void drawRow(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    void currentChanged(const QModelIndex&, const QModelIndex&) override;

signals:

    void currentChanged(const QModelIndex&);

};
