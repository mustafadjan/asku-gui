#include "CtrlParamTableView.h"
#include "CtrlParamTableProxyModel.h"
#include "AbstractSchemeTreeItem.h"
#include <QHeaderView>

CtrlParamTableView::CtrlParamTableView(QAbstractItemModel* sourceModel, QWidget* parent):
    QTableView(parent)
{
    setModel(new CtrlParamTableProxyModel(sourceModel, this));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setSelectionMode(QAbstractItemView::NoSelection);
    verticalHeader()->hide();
}

void CtrlParamTableView::setRootIndex(const QModelIndex& index)
{
    auto proxyModel = qobject_cast<const SchemeBaseProxyModel*>(model());

    int rowCount = proxyModel->rowCount(rootIndex());

    for (int i = 0; i < rowCount; ++i) {
        setRowHidden(i, false);
    }

    QTableView::setRootIndex(proxyModel->mapFromSource(index));

    auto condition = [] (AbstractSchemeTreeItem* item) -> bool {
        return item->hasChild();
    };

    for (const auto row : proxyModel->rowsToHide({ItemType::CtrlParam}, rootIndex(), condition)) {
        setRowHidden(row, true);
    }

    resizeColumnsToContents();
}

void CtrlParamTableView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight,
                                     const QVector<int>& roles)
{
    QTableView::dataChanged(topLeft, bottomRight, roles);

    if (topLeft == bottomRight && topLeft.isValid()) {
        viewport()->update();
    }
}
