#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include "VoiTypes.h"

class TuneParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(TuneParamTreeItemPrivate)

public:

    TuneParamData data{};
    QVariant outerValue;

    QString description, unit;
    //QPair<float, float> range{std::numeric_limits<float>::lowest(),
    //                          std::numeric_limits<float>::max()};
    QVariant min;
    QVariant max;
    QStringList values;
    bool saveProp{false}, valueChanged{false};

    explicit TuneParamTreeItemPrivate(quint32 id, const QString& name,
                                      AbstractSchemeTreeItem* parent):
        AbstractSchemeTreeItemPrivate(id, name, parent)
    {
    }

    QVariant value() const { return value(data.value); }
    QVariant getOuterValue() const { return value(outerValue); }

private:

    QVariant value(const QVariant& value) const
    {
        if (unit.isEmpty()) {
            if (values.isEmpty()) {
                return value;
            }

            return values[value.toInt()];
        }

        // todo: make smooth (QVariant::toString() leads to digit excess)
        return QString("%1 %2").arg(value.toString(), unit);
    }

};
