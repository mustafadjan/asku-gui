#pragma once

#include <QSortFilterProxyModel>

enum class ModelType : quint8;
enum class ItemType : quint8;
class AbstractSchemeTreeItem;
/**
 * @brief Базовая прокси-модель, фильтрует элементы по типу модели
 */
class SchemeBaseProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    explicit SchemeBaseProxyModel(ModelType, QAbstractItemModel*, QObject* = nullptr);

    QVector<int> rowsToHide(QList<ItemType>, const QModelIndex&, bool (*)(AbstractSchemeTreeItem*)
                            = [] (AbstractSchemeTreeItem*) { return false; }) const;

protected:

    bool filterAcceptsRow(int, const QModelIndex&) const override;

private:

    ModelType modelType;

};
