#pragma once

#include <QMetaType>

namespace Qt {
    // собственные роли
    enum // ItemDataRole
    {
        ConditionRole = BackgroundRole,
        StateRole = DecorationRole,
        ChangeTPRole = FontRole,
        FullPathRole = UserRole + 1,
        IDRole,
        TimeRole,
        ValueRole,
        LocalRole,
        ImitRole,
        NameRole,
        IsModuleRole,
        DescriptionRole,
        FormatRole,
        TemplateRole,
        ResetTPRole,
        ElemFlagsRole
    };
}
// тип представления
enum class ModelType : quint8
{
    SourceTree,
    ElemTree,
    CtrlParamTable,
    TuneParamTree,
    ElemList
};
// тип узла
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
/**
 * @brief Базовый абстрактный класс для всех узлов
 */
class AbstractSchemeTreeItem
{
    Q_DECLARE_PRIVATE_D(d, AbstractSchemeTreeItem)

public:

    virtual ~AbstractSchemeTreeItem();

    AbstractSchemeTreeItem* child(int) const;
    bool hasChild() const;
    int childCount() const;
    int row() const;
    AbstractSchemeTreeItem* parentItem() const;
    AbstractSchemeTreeItem* rootItem() const;
    AbstractSchemeTreeItem* preRootItem() const;
    ItemType type() const;
    quint32 id() const;
    void setName(const QString&);
    void removeChilds();

    // ролевые данные (полный путь, id, состояние/статус и т.д.)
    virtual QVariant roleData(int) const;

    // количество столбцов, которые возможно показать в представлении
    virtual int columnCount() const = 0;
    // данные для отображения в столбце представления (имя, значение, время и т.д.)
    virtual QVariant data(int) const = 0;
    // признак валидности для конкретного представления
    virtual bool isValid(ModelType) const = 0;
    // установка новых данных, возвращает роли, данные по которым были изменены
    virtual QVector<int> setData(const QVariant&) = 0;

protected:

    explicit AbstractSchemeTreeItem(AbstractSchemeTreeItemPrivate&);

    AbstractSchemeTreeItemPrivate* const d;

};
