#include "PathCompleter.h"
#include "AbstractSchemeTreeItem.h"
#include <QAbstractProxyModel>
#include <QMouseEvent>
#include <QDebug>
#include <QListView>

PathCompleter::PathCompleter(QAbstractItemModel* model, QObject* parent):
    QCompleter(model, parent)
{
    setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    setCompletionRole(Qt::DisplayRole);
    //setMaxVisibleItems(10);

    // зачем-то приходит индекс внутренней прокси-модели QCompleter'а, переводим его в наш исходный
    connect(this, QOverload<const QModelIndex&>::of(&QCompleter::activated),
            this, &PathCompleter::convertToSource);

    //popup()->removeEventFilter(this);
    //popup()->installEventFilter(this);

    //connect(model, &QAbstractItemModel::dataChanged, popup(), &QAbstractItemView::setCurrentIndex);

    //connect(this, QOverload<const QModelIndex&>::of(&QCompleter::highlighted),
    //        this, [this] (const QModelIndex& index)
    //{
    //    qDebug() << __PRETTY_FUNCTION__ << index << popup();
    //});
}

QModelIndex PathCompleter::mapFromSource(const QModelIndex& sourceIndex) const
{
    return qobject_cast<const QAbstractProxyModel*>(model())->mapFromSource(sourceIndex);
}

QString PathCompleter::pathFromIndex(const QModelIndex& index) const
{
    return model()->data(index, Qt::FullPathRole).toString();
}

QStringList PathCompleter::splitPath(const QString& path) const
{
    return path.split('/');
}

bool PathCompleter::eventFilter(QObject* o, QEvent* e)
{
    //if (e->type() == QEvent::UpdateRequest) {
    //    auto index = popup()->indexAt(popup()->mapFromGlobal(QCursor::pos()));
    //    qDebug() << __PRETTY_FUNCTION__ << popup() << o << QCursor::pos() << index;
    //    if (index.isValid()) {
    //        popup()->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
    //    }
    //}

    return QCompleter::eventFilter(o, e);
}

void PathCompleter::convertToSource(const QModelIndex& index)
{
    emit activated(qobject_cast<const QAbstractProxyModel*>(index.model())->mapToSource(index));
}
