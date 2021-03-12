#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include "VoiTypes.h"

class CtrlParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(CtrlParamTreeItemPrivate)

public:

    CtrlParamData data{};

    QString description, format;
    QHash<int, QString> templates;

    explicit CtrlParamTreeItemPrivate(quint32 id, const QString& name,
                                      AbstractSchemeTreeItem* parent):
        AbstractSchemeTreeItemPrivate(id, name, parent)
    {
    }

    QVariant value() const
    {
        bool b;
        int i = data.value.toInt(&b);

        if (!data.value.isNull() && b && !templates[i].isEmpty()) {
            return QString("[%1] ").arg(i).append(templates[i]);
        }
        else if (!format.isEmpty()) {
            return QString::asprintf(format.toUtf8().constData(), data.value.toDouble());
        }

        return data.value;
    }

};
