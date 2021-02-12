#include "TuneParamTreeView.h"
#include "TuneParamTreeProxyModel.h"
//#include "tuneparamitemdelegate.h"
#include "AbstractSchemeTreeItem.h"
#include <QMenu>
#include <QLineEdit>
#include <QItemEditorFactory>
#include <QDebug>

TuneParamTreeView::TuneParamTreeView(QAbstractItemModel* sourceModel, QWidget* parent):
    QTreeView(parent)
{
    auto itemEditorFactory = new QItemEditorFactory;

    auto listCreator = new QStandardItemEditorCreator<QLineEdit>();

    itemEditorFactory->registerEditor(QVariant::String, listCreator);

    QItemEditorFactory::setDefaultFactory(itemEditorFactory);

    auto tuneParamTreeModel = new TuneParamTreeProxyModel(sourceModel, this);
    setModel(tuneParamTreeModel);
    //setEditTriggers(QAbstractItemView::NoEditTriggers);
    //setItemDelegateForColumn(2, new TuneParamItemDelegate(tuneParamTreeModel));
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAnimated(true);
    connect(this, &QAbstractItemView::doubleClicked, this, [this] (const QModelIndex& index)
    {
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

void TuneParamTreeView::sendChangedTP()
{
    auto sourceIndex = qobject_cast<const QAbstractProxyModel*>(model())->mapToSource(rootIndex());
    if (sourceIndex.isValid()) {
        //auto item =
    }
}

void TuneParamTreeView::contextMenu(const QPoint& pos)
{
    qDebug() << __PRETTY_FUNCTION__;
    auto index = qobject_cast<const QAbstractProxyModel*>(model())->mapToSource(indexAt(pos));
    if (index.isValid()) {
        QMenu menu;
        menu.addAction(tr("Сбросить устанавливаемое значение"), this, [this, pos]
        {
            //auto item = static_cast<AbstractSchemeTreeItem*>(index.internalPointer());
            //auto tuneParamItem = dynamic_cast<TuneParamTreeItem*>(item);
            //if (tuneParamItem) {
                //tuneParamItem->resetOuterData();
                model()->setData(indexAt(pos), QVariant(), Qt::ResetTPRole);
            //}
        });
        menu.addSeparator();
        menu.addAction(tr("Сбросить все устанавливаемые значения"));
        menu.exec(viewport()->mapToGlobal(pos));
    }
}
