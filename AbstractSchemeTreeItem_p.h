#pragma once

#include <QVector>

enum class ItemType : quint8;
class AbstractSchemeTreeItem;

class AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(AbstractSchemeTreeItemPrivate)

public:

    ItemType itemType;
    quint32 id{0};
    QString name;

    QVector<AbstractSchemeTreeItem*> childItems;
    AbstractSchemeTreeItem* parentItem{nullptr};

protected:

    explicit AbstractSchemeTreeItemPrivate(quint32 id, const QString& name,
                                           AbstractSchemeTreeItem* parent = nullptr):
        id(id),
        name(name),
        parentItem(parent)
    {
    }

};
