#pragma once

#include "AbstractSchemeTreeItem_p.h"
#include "VoiTypes.h"

class CtrlParamTreeItemPrivate : public AbstractSchemeTreeItemPrivate
{
    Q_DISABLE_COPY(CtrlParamTreeItemPrivate)

public:

    qint64 time2{0};
    CtrlParamData data;

    explicit CtrlParamTreeItemPrivate(ItemType, quint32, const QString&, AbstractSchemeTreeItem*);

};
