#include "TuneParamTreeView.h"
#include "TuneParamTreeProxyModel.h"
#include "TuneParamItemDelegate.h"
#include "AbstractElemTreeItem.h"
#include "TuneParamTreeItem.h"
#include "VoiTypes.h"
#include <QDateTime>
#include <QMenu>
#include <QDebug>

TuneParamTreeView::TuneParamTreeView(QAbstractItemModel* sourceModel, QWidget* parent):
    QTreeView(parent)
{
    auto tuneParamTreeModel = new TuneParamTreeProxyModel(sourceModel, this);
    setModel(tuneParamTreeModel);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setItemDelegateForColumn(2, new TuneParamItemDelegate(tuneParamTreeModel));
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAnimated(true);
    connect(this, &QAbstractItemView::doubleClicked, this, [this] (const QModelIndex& index) {
        if (index.column() == 2) {
            edit(index);
        }
    });
    connect(this, &QWidget::customContextMenuRequested, this, &TuneParamTreeView::contextMenu);

    hideColumn(2);
}

void TuneParamTreeView::setRootIndex(const QModelIndex& index)
{
    QTreeView::setRootIndex(qobject_cast<const QAbstractProxyModel*>(model())->
                            mapFromSource(index));
}

void TuneParamTreeView::sendNewValues()
{
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    auto values = qobject_cast<const TuneParamTreeProxyModel*>(model())->getNewValues(rootIndex());
    for (auto& value : values) {
        value.m_oneConfParam.m_isSaveCommand = false;
    }
    auto index = qobject_cast<const QAbstractProxyModel*>(model())->mapToSource(rootIndex());
    auto elemTreeItem = dynamic_cast<AbstractElemTreeItem*>(
                        static_cast<AbstractSchemeTreeItem*>(index.internalPointer()));
    if (elemTreeItem) {
        emit newValues(elemTreeItem->rlkId(), elemTreeItem->moduleId(), time, values);
    }
}
// todo: merge these methods
void TuneParamTreeView::sendNewValuesSaveProp()
{
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    auto values = qobject_cast<const TuneParamTreeProxyModel*>(model())->getNewValues(rootIndex());
    auto index = qobject_cast<const QAbstractProxyModel*>(model())->mapToSource(rootIndex());
    auto elemTreeItem = dynamic_cast<AbstractElemTreeItem*>(
                        static_cast<AbstractSchemeTreeItem*>(index.internalPointer()));
    if (elemTreeItem) {
        emit newValues(elemTreeItem->rlkId(), elemTreeItem->moduleId(), time, values);
    }
}

void TuneParamTreeView::contextMenu(const QPoint& pos)
{
    qDebug() << __PRETTY_FUNCTION__;
    auto index = qobject_cast<const QAbstractProxyModel*>(model())->mapToSource(indexAt(pos));
    if (index.isValid()) {
        QMenu menu;
        menu.addAction(tr("Сбросить устанавливаемое значение"), this, [this, pos, index] {
            auto item = static_cast<AbstractSchemeTreeItem*>(index.internalPointer());
            auto tuneParamItem = dynamic_cast<TuneParamTreeItem*>(item);
            if (tuneParamItem) {
                tuneParamItem->resetOuterData();
                model()->setData(indexAt(pos), QVariant(), Qt::ResetTPRole);
            }
        });
        menu.addSeparator();
        menu.addAction(tr("Сбросить все устанавливаемые значения"));
        menu.exec(viewport()->mapToGlobal(pos));
    }
}
