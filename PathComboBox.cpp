#include "PathComboBox.h"
#include "PathLineEdit.h"

PathComboBox::PathComboBox(QAbstractItemModel* model, QWidget* parent):
    QComboBox(parent)
{
    setEditable(true);
    //setContextMenuPolicy(Qt::NoContextMenu);
    auto pathLineEdit = new PathLineEdit(model, this);
    setLineEdit(pathLineEdit);
    //setMaxVisibleItems(10);

    connect(this, QOverload<int>::of(&QComboBox::activated), this, &PathComboBox::activate);
    connect(pathLineEdit, &PathLineEdit::activated, this, &PathComboBox::activated);
}

void PathComboBox::updateItem(const QModelIndex& index)
{
    auto pathlineEdit = qobject_cast<const PathLineEdit*>(lineEdit());
    QPersistentModelIndex proxyIndex(pathlineEdit->mapFromSource(index));
    removeItem(findData(proxyIndex));
    insertItem(0, pathlineEdit->fullPath(proxyIndex), proxyIndex);
    setCurrentIndex(0);
}

void PathComboBox::activate(int index)
{
    emit activated(itemData(index).toModelIndex());
}
