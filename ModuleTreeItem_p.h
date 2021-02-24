#pragma once

#include "AbstractElemTreeItem_p.h"

class RLKTreeItem;

class ModuleTreeItemPrivate : public AbstractElemTreeItemPrivate
{
    Q_DISABLE_COPY(ModuleTreeItemPrivate)

public:

    quint8 RGDV:1;
    quint8 RAF:1;
    quint8 hasTP:1;
    quint8 TP:1;
    quint16 mode;

    explicit ModuleTreeItemPrivate(quint32, RLKTreeItem*);

};
