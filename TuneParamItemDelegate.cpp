#include "TuneParamItemDelegate.h"
#include "TuneParamTreeItem.h"
#include <QAbstractProxyModel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDebug>

TuneParamItemDelegate::TuneParamItemDelegate(QObject* parent):
    QStyledItemDelegate(parent)
{
}

QWidget* TuneParamItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                             const QModelIndex& index) const
{
    if (index.column() == 2) {
        auto tuneParamItem = getTuneParamItem(index);
        if (tuneParamItem) {
            qDebug() << __PRETTY_FUNCTION__;
            return tuneParamItem->createEditor(parent);
        }
    }

    return nullptr;
}

void TuneParamItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
}

void TuneParamItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                         const QModelIndex& index) const
{
    qDebug() << __PRETTY_FUNCTION__  << editor << editor->devType();
    auto tuneParamItem = getTuneParamItem(index);
    if (tuneParamItem) {
        model->setData(index, tuneParamItem->editorData(editor));
    }
}

void TuneParamItemDelegate::updateEditorGeometry(QWidget* editor,
                                                 const QStyleOptionViewItem& option,
                                                 const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

TuneParamTreeItem* TuneParamItemDelegate::getTuneParamItem(const QModelIndex& index) const
{
    auto sourceIndex = qobject_cast<const QAbstractProxyModel*>(index.model())->
                       mapToSource(index);
    auto item = static_cast<AbstractSchemeTreeItem*>(sourceIndex.internalPointer());
    return dynamic_cast<TuneParamTreeItem*>(item);
}
