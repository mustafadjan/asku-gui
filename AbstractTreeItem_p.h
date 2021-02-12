#pragma once

#include <QVector>

class AbstractTreeItem;

class AbstractTreeItemPrivate
{
    Q_DISABLE_COPY(AbstractTreeItemPrivate)

public:

    QString name;

    QVector<AbstractTreeItem*> childItems;
    AbstractTreeItem* parentItem{nullptr};

protected:

    explicit AbstractTreeItemPrivate(const QString& name, AbstractTreeItem* parent = nullptr):
        name(name),
        parentItem(parent)
    {
    }

};
