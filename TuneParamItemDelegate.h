#pragma once

#include <QStyledItemDelegate>

class TuneParamTreeItem;

class TuneParamItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    explicit TuneParamItemDelegate(QObject* = nullptr);

    QWidget* createEditor(QWidget*, const QStyleOptionViewItem&,
                          const QModelIndex&) const override;
    void setEditorData(QWidget*, const QModelIndex&) const override;
    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const override;
    void updateEditorGeometry(QWidget*, const QStyleOptionViewItem&,
                              const QModelIndex&) const override;
private:

    TuneParamTreeItem* getTuneParamItem(const QModelIndex&) const;

};
