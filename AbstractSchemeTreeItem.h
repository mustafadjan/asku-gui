#pragma once

#include "AbstractTreeItem.h"

namespace Qt {
    enum // ItemDataRole
    {
        ConditionRole = BackgroundRole,
        StateRole = DecorationRole,
        ChangeTPRole = FontRole,
        FullPathRole = UserRole + 1,
        IDRole,
        TimeRole,
        IsModuleRole,
        ResetTPRole,
        ElemFlagsRole
    };
}

enum class ModelType : quint8
{
    SourceTree,
    ElemTree,
    CtrlParamTable,
    TuneParamTree,
    ElemList
};

enum class ItemType : quint8
{
    RLK,
    Module,
    Elem,
    CtrlParam,
    TuneParam
};
Q_DECLARE_METATYPE(ItemType)

class AbstractSchemeTreeItemPrivate;

class AbstractSchemeTreeItem : public AbstractTreeItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractSchemeTreeItem)

public:

    //AbstractSchemeTreeItem* child(int) const;
    //bool hasChild() const;
    //int childCount() const;
    //virtual int columnCount() const = 0;
    //virtual QVariant data(int) const = 0;
    //virtual QVariant roleData(int) const = 0;
    //virtual QVector<int> setData(const QVariant&) = 0;
    //int row() const;
    //AbstractSchemeTreeItem* parentItem() const;
    //AbstractSchemeTreeItem* rootItem() const;
    AbstractSchemeTreeItem* child(int) const;
    int row() const;
    AbstractSchemeTreeItem* parentItem() const;
    AbstractSchemeTreeItem* rootItem() const;
    ItemType type() const;
    quint32 id() const;

    virtual QVariant roleData(int) const;
    virtual bool isValid(ModelType) const = 0;

protected:

    explicit AbstractSchemeTreeItem(AbstractSchemeTreeItemPrivate&);
    //virtual ~AbstractSchemeTreeItem();

    //AbstractSchemeTreeItemPrivate* const d;

};
