#pragma once

#include <QSortFilterProxyModel>

enum class ModelType : quint8;
enum class ItemType : quint8;
class AbstractSchemeTreeItem;
/**
 * @brief Базовая прокси-модель, фильтрует элементы по типу представления
 */
class SchemeBaseProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    explicit SchemeBaseProxyModel(ModelType, QAbstractItemModel*, QObject* = nullptr);

    // сообщает какие строки необходимо спрятать в представлении
    // почему бы их не отфильтровать как другие? в каких-то представлениях некоторые узлы требуются
    // как родители целевых узлов и только (устанавливая индекс узла как корень представления,
    // показываются его потомки), чтобы такой узел не был виден, когда корнем установлен его
    // предок, необходимо его спрятать
    // последним параметром передается callback дополнительного условия (например наличие потомков)
    QVector<int> rowsToHide(QList<ItemType>, const QModelIndex&, bool (*)(AbstractSchemeTreeItem*)
                            = [] (AbstractSchemeTreeItem*) { return false; }) const;

protected:

    // фильтрация узлов, используется для всех производных проксирующих классов
    bool filterAcceptsRow(int, const QModelIndex&) const override;

private:

    ModelType modelType;

};
