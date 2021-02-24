#include "ElemTreeView.h"
#include "ElemTreeProxyModel.h"
#include "GroupingModulesProxyModel.h"
#include <QDebug>

ElemTreeView::ElemTreeView(QAbstractItemModel* sourceModel, QWidget* parent):
    QTreeView(parent)
{
    setModel(new ElemTreeProxyModel(sourceModel, this));
    //setModel(new GroupingModulesProxyModel(new ElemTreeProxyModel(sourceModel, this), this));
    //expandToDepth(1);
    setRootIsDecorated(false);
    setAnimated(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setSelectionMode(QAbstractItemView::NoSelection);

    //connect(model(), &QAbstractItemModel::dataChanged, this, [this] (const QModelIndex& proxyIndex1,
    //        const QModelIndex& proxyIndex2, const QVector<int>& roles)
    //{
    //    qDebug() << proxyIndex1;
    //    auto proxyModel = qobject_cast<const QAbstractProxyModel*>(QTreeView::model());
    //    auto index1 = proxyModel->mapToSource(proxyIndex1);
    //    auto index2 = proxyModel->mapToSource(proxyIndex2);
    //    emit dataChanged(index1, index2, roles);
    //});
    //connect(model(), &QAbstractItemModel::rowsInserted, this, [this] (const QModelIndex& proxyIndex,
    //        int first, int last)
    //{
    //    qDebug() << proxyIndex<< first << last;
    //    auto proxyModel = qobject_cast<const QAbstractProxyModel*>(QTreeView::model());
    //    auto index = proxyModel->mapToSource(proxyIndex);
    //    rowsInserted(index, first, last);
    //});
}

//QAbstractItemModel* ElemTreeView::model() const
//{
//    return qobject_cast<const QAbstractProxyModel*>(QTreeView::model())->sourceModel();
//}

void ElemTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& options,
                           const QModelIndex& index) const
{
    //qDebug() << __PRETTY_FUNCTION__ << i++ << QTreeView::model()->data(index)
    //         << ((options.state & QStyle::State_Enabled) != 0);
    QTreeView::drawRow(painter, options, index);
    auto opt = options;
    opt.state &= QStyle::State_Enabled;
    QPalette::ColorGroup cg;
    cg = QPalette::Active;
    opt.palette.setCurrentColorGroup(cg);
    //itemDelegate(index)->paint(painter, opt, index);
}

void ElemTreeView::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QTreeView::currentChanged(current, previous);

    if (current != previous) {
        auto proxyModel = qobject_cast<const QAbstractProxyModel*>(QTreeView::model());
        auto proxyIndex = proxyModel->mapToSource(current);

        //qDebug() << __PRETTY_FUNCTION__ << current << proxyIndex;
        //emit currentChanged(qobject_cast<const QAbstractProxyModel*>(proxyModel->sourceModel())->
        //                    mapToSource(proxyIndex));
        emit currentChanged(proxyIndex);
    }
}
