#pragma once

#include <QMetaType>

namespace Qt {
    enum // ItemDataRole
    {
        ConditionRole = BackgroundRole,
        StateRole = DecorationRole,
        ChangeTPRole = FontRole,
        FullPathRole = UserRole + 1,
        IDRole,
        TimeRole,
        NameRole,
        IsModuleRole,
        DescriptionRole,
        FormatRole,
        TemplateRole,
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

class AbstractSchemeTreeItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractSchemeTreeItem)

    friend class SchemeTreeModelPrivate;
public:

    AbstractSchemeTreeItem* child(int) const;
    bool hasChild() const;
    int childCount() const;
    int row() const;
    AbstractSchemeTreeItem* parentItem() const;
    AbstractSchemeTreeItem* rootItem() const;
    ItemType type() const;
    quint32 id() const;
    void setName(const QString&);
    void removeChilds();

    virtual QVariant roleData(int) const;

    virtual int columnCount() const = 0;
    virtual QVariant data(int) const = 0;
    virtual bool isValid(ModelType) const = 0;
    virtual QVector<int> setData(const QVariant&) = 0;

protected:

    explicit AbstractSchemeTreeItem(AbstractSchemeTreeItemPrivate&);
    virtual ~AbstractSchemeTreeItem();

    AbstractSchemeTreeItemPrivate* const d;

};
