#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include "VoiTypes.h"

class CtrlParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(CtrlParamTreeItemPrivate)

public:

    CtrlParamData data;

    QString description, format;
    QHash<int, QString> templates;

    explicit CtrlParamTreeItemPrivate(quint32, const QString&, AbstractSchemeTreeItem*);

};
