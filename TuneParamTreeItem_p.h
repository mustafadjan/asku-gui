#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include <QVariant>
#include <QSet>

class TuneParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(TuneParamTreeItemPrivate)

public:

    QVariant innerData;
    QVariant outerData;
    QPair<float, float> range{};
    QSet<QString> valueSet;
    bool dataChanged{false};

    explicit TuneParamTreeItemPrivate(ItemType itemType, quint32 id, const QString& name,
                                      AbstractSchemeTreeItem* parent):
        AbstractSchemeTreeItemPrivate(itemType, id, name, parent)
    {
    }

};
