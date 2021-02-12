#pragma once

#include <QAbstractItemModel>

struct Pack0x24;
struct ElemData;
struct CtrlParamData;
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
    void buildScheme(quint32, const QHash<quint32, QJsonDocument>&);
    void updateRlkData(const QHash<quint32, Pack0x24>&) const;
    void updateModuleData(quint32, const QHash<quint32, Pack0x24>&) const;
    void updateElemsData(quint32, quint32, const QHash<quint16, ElemData>&) const;
    void updateCtrlParamsData(quint32, quint32, const QHash<quint32, CtrlParamData>&) const;
    void updateTuneParamsData(quint32, quint32, const QHash<quint32, QVariant>&) const;

private:

    QScopedPointer<SchemeTreeModelPrivate> d;

signals:

    void constDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&) const;

};
