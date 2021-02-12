#include "ElemListView.h"
#include "ElemListProxyModel.h"
#include "AbstractSchemeTreeItem.h"

ElemListView::ElemListView(QAbstractItemModel* sourceModel, QWidget* parent):
    QListView(parent)
{
    setModel(new ElemListProxyModel(sourceModel, this));
    setViewMode(QListView::IconMode);
    setFlow(QListView::TopToBottom);
    setResizeMode(QListView::Adjust);
    setBatchSize(50);
    setLayoutMode(QListView::Batched);
    setSpacing(10);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void ElemListView::setRootIndex(const QModelIndex& index)
{
    auto proxyModel = qobject_cast<const SchemeBaseProxyModel*>(model());

    int rowCount = proxyModel->rowCount(rootIndex());

    for (int i = 0; i < rowCount; ++i) {
        if (isRowHidden(i)) {
            setRowHidden(i, false);
        }
    }

    QListView::setRootIndex(proxyModel->mapFromSource(index));

    for (const auto row : proxyModel->rowsToHide({ItemType::Module,ItemType::Elem}, rootIndex())) {
        setRowHidden(row, true);
    }
}
