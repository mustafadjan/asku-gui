#include "CtrlParamTreeItem.h"
#include "CtrlParamTreeItem_p.h"
#include <QDateTime>
#include <QBrush>

CtrlParamTreeItem::CtrlParamTreeItem(quint32 id, const QString& name,
                                     AbstractSchemeTreeItem* parent):
    AbstractSchemeTreeItem(*new CtrlParamTreeItemPrivate(id, name, parent))
{
    d_func()->itemType = ItemType::CtrlParam;
}

int CtrlParamTreeItem::columnCount() const
{
    return 5;
}

QVariant CtrlParamTreeItem::data(int column) const
{
    switch (column) {
        case 0:
            return d_func()->name;
        case 1:
            return d_func()->value();
        case 2:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeRecv).toUTC()
                                                  .toString("hh:mm:ss");
        case 3:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeCond).toUTC()
                                                  .toString("hh:mm:ss");
        case 4:
            return QDateTime::fromMSecsSinceEpoch(d_func()->data.timeVal).toUTC()
                                                  .toString("hh:mm:ss");
    }

    return QVariant();
}

QVariant CtrlParamTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::ConditionRole:
            return conditionBrush();
    }

    return AbstractSchemeTreeItem::roleData(role);
}

QVector<int> CtrlParamTreeItem::setData(const QVariant& data)
{
    if (data.canConvert<CtrlParamData>()) {
        auto inData = data.value<CtrlParamData>();
        // подразумевается, что значение КП приходит в "сыром" виде (QByteArray)...
        auto rawValue = inData.value.toByteArray();
        // ...поэтому нормализуем значение в соответствии с уже имеющимся типом
        switch (d_func()->data.value.userType()) {
            case QMetaType::Float:
                inData.value = *reinterpret_cast<const float*>(rawValue.constData());
                break;
            case QMetaType::QString:
                inData.value = QString::fromUtf8(rawValue);
                break;
            case QMetaType::Int:
                inData.value = *reinterpret_cast<const int*>(rawValue.constData());
                break;
            case QMetaType::Bool:
                inData.value = *reinterpret_cast<const bool*>(rawValue.constData());
                break;
            default:
                inData.value = QVariant(QVariant::Type(QMetaType::UnknownType));
        }
        d_func()->data = inData;
        return {Qt::ConditionRole, Qt::TimeRole, Qt::ValueRole};
    }

    return {};
}

bool CtrlParamTreeItem::isValid(ModelType modelType) const
{
    switch (modelType) {
        case ModelType::ElemTree:
            return hasChild();
        case ModelType::CtrlParamTable:
            return true;
        default:
            return false;
    }
}

void CtrlParamTreeItem::setDescription(const QString& description)
{
    d_func()->description = description;
}

void CtrlParamTreeItem::setFormat(const QString& format)
{
    d_func()->format = format;
}

void CtrlParamTreeItem::setTemplates(const QHash<int, QString> templates)
{
    d_func()->templates = templates;
}

EModulState CtrlParamTreeItem::condition() const
{
    return static_cast<EModulState>(d_func()->data.condition);
}

void CtrlParamTreeItem::setValueType(QMetaType::Type type)
{
    // если КП - группа, то ему ставится статус "норма" (по умолчанию "неизвестно")
    // для нормальной подсветки, т.к. его статус не будет обновляться
    if (type == QMetaType::User) {
        setData(QVariant::fromValue(CtrlParamData{eStNorma, 0, 0, 0, QVariant()}));
    }

    d_func()->data.value = QVariant(QVariant::Type(type));
}
