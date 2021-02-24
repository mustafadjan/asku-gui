#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include <QVariant>

class TuneParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(TuneParamTreeItemPrivate)

public:

    qint64 time;
    QVariant innerData;
    QVariant outerData;

    QString description, unit;
    QVariant defaultValue;
    QPair<float, float> range{std::numeric_limits<float>::lowest(),
                              std::numeric_limits<float>::max()};
    QVector<QString> values;
    bool saveStatus{false}, dataChanged{false};

    explicit TuneParamTreeItemPrivate(quint32 id, const QString& name,
                                      AbstractSchemeTreeItem* parent):
        AbstractSchemeTreeItemPrivate(id, name, parent)
    {
    }

};
