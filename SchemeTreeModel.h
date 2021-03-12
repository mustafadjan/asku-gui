#pragma once

#include <QAbstractItemModel>

struct Pack0x24;
struct ElemData;
struct CtrlParamData;
struct TuneParamData;
class SchemeTreeModelPrivate;

class SchemeTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE_D(d, SchemeTreeModel)
    Q_DISABLE_COPY(SchemeTreeModel)

public:

    explicit SchemeTreeModel(QObject* = nullptr);
    ~SchemeTreeModel() override;

    QModelIndex index(int, int, const QModelIndex& = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;

public slots:

    // 0xD001
    void buildSchemes(quint32, const QHash<quint32, QJsonValue>&);
    void updateRlk(quint32, const Pack0x24&) const;
    void updateModules(quint32, const QHash<quint32, Pack0x24>&) const;
    void updateElems(quint32, const QHash<quint32, QHash<quint16, ElemData>>&) const;
    void updateCtrlParams(quint32, const QHash<quint32, QHash<quint32, CtrlParamData>>&) const;
    void updateTuneParams(quint32, const QHash<quint32, QHash<quint32, TuneParamData>>&) const;

private:

    SchemeTreeModelPrivate* const d;

signals:

    void constDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&) const;

};
