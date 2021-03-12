#include "TuneParamTreeItem.h"
#include "TuneParamTreeItem_p.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>

TuneParamTreeItem::TuneParamTreeItem(quint32 id, const QString& name,
                                     AbstractSchemeTreeItem* parent):
    AbstractSchemeTreeItem(*new TuneParamTreeItemPrivate(id, name, parent))
{
    d_func()->itemType = ItemType::TuneParam;
}

int TuneParamTreeItem::columnCount() const
{
    return 3;
}

QVariant TuneParamTreeItem::data(int column) const
{
    switch (column) {
        case 0:
            return d_func()->name;
        case 1:
            return d_func()->value();
        case 2:
            return d_func()->getOuterValue();
    }

    return QVariant();
}

QVariant TuneParamTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::ChangeTPRole:
            auto font = QFont();
            return QFont(font.family(), font.pointSize(),
                         d_func()->valueChanged ? QFont::Bold : font.weight(), font.italic());
    }

    return AbstractSchemeTreeItem::roleData(role);
}

QVector<int> TuneParamTreeItem::setData(const QVariant& data)
{
    if (data.canConvert<TuneParamData>()) { // установка нового значения по сообщению
        auto inData = data.value<TuneParamData>();
        auto rawValue = inData.value.toByteArray();
        switch (d_func()->data.value.userType()) {
            case QMetaType::Float:
                inData.value = *reinterpret_cast<const float*>(rawValue.constData());
                break;
            case QMetaType::QString:
                inData.value = QString::fromUtf8(rawValue);
                break;
            case QMetaType::Int:
                inData.value = int(*reinterpret_cast<const qint32*>(rawValue.constData()));
                break;
            case QMetaType::Bool:
                inData.value = bool(*reinterpret_cast<const qint8*>(rawValue.constData()));
                break;
            default:
                inData.value = QVariant(QVariant::Type(QMetaType::UnknownType));
        }
        d_func()->data = inData;
        if (!d_func()->valueChanged) {
            d_func()->outerValue = d_func()->data.value;
        }
        return {Qt::TimeRole, Qt::ValueRole};
    }

    switch (data.userType()) { // установка нового значения пользователем
        case QMetaType::Float:
        case QMetaType::QString:
        case QMetaType::Int:
        case QMetaType::Bool:
            d_func()->outerValue = data;
            d_func()->valueChanged = (d_func()->data.value != data);
            return {Qt::ChangeTPRole};
    }

    return {};
}

bool TuneParamTreeItem::isValid(ModelType modelType) const
{
    return modelType == ModelType::TuneParamTree;
}

void TuneParamTreeItem::setUnit(const QString& unit)
{
    d_func()->unit = unit;
}

void TuneParamTreeItem::setSaveProp(bool b)
{
    d_func()->saveProp = b;
}

void TuneParamTreeItem::setMin(float min)
{
    d_func()->min = min;
}

void TuneParamTreeItem::setMax(float max)
{
    d_func()->max = max;
}

void TuneParamTreeItem::setValues(const QStringList& values)
{
    d_func()->values = values;
}

void TuneParamTreeItem::setDescription(const QString& description)
{
    d_func()->description = description;
}

bool TuneParamTreeItem::isNewValue() const
{
    return d_func()->valueChanged;
}

OneConfigParamValue TuneParamTreeItem::packaging() const
{
    // заполнение value в денормализованном виде (как прием в setData)
    switch (d_func()->outerValue.userType()) {
        case QMetaType::Float:
        {
            float tmp = d_func()->outerValue.toFloat();
            auto value = QByteArray(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
            return {{id(), quint32(value.size()), d_func()->saveProp, 0}, value};
        }
        case QMetaType::QString:
        {
            auto value = d_func()->outerValue.toString().toUtf8();
            return {{id(), quint32(value.size()), d_func()->saveProp, 0}, value};
        }
        case QMetaType::Int:
        {
            qint32 tmp = d_func()->outerValue.toInt();
            auto value = QByteArray(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
            return {{id(), quint32(value.size()), d_func()->saveProp, 0}, value};
        }
        case QMetaType::Bool:
        {
            qint8 tmp = d_func()->outerValue.toBool();
            auto value = QByteArray(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
            return {{id(), quint32(value.size()), d_func()->saveProp, 0}, value};
        }
    }

    return OneConfigParamValue();
}

QWidget* TuneParamTreeItem::createEditor(QWidget* parent) const
{
    switch (d_func()->outerValue.userType()) {
        case QMetaType::Float:
        {
            auto editor = new QDoubleSpinBox(parent);
            if (d_func()->min.toInt() <= d_func()->max.toInt()) {
                editor->setRange(d_func()->min.toInt(), d_func()->max.toInt());
            }
            editor->setValue(d_func()->outerValue.toDouble());
            return editor;
        }
        case QMetaType::QString:
            return new QLineEdit(d_func()->outerValue.toString(), parent);
        case QMetaType::Int:
            if (d_func()->values.isEmpty()) {
                auto editor = new QSpinBox(parent);
                if (d_func()->min.toInt() <= d_func()->max.toInt()) {
                    editor->setRange(d_func()->min.toInt(), d_func()->max.toInt());
                }
                editor->setValue(d_func()->outerValue.toInt());
                return editor;
            }
            else {
                auto editor = new QComboBox(parent);
                editor->addItems(d_func()->values);
                editor->setCurrentIndex(d_func()->outerValue.toInt());
                return editor;
            }
        //case QMetaType::Bool:
        //case QMetaType::User:
        //    if (d_func()->outerData.userType() == 1) {
        //        auto editor = new QComboBox(parent);
        //        editor->addItems(d_func()->outerData.value<StackedTuneParamValue>.toList());
        //        editor->setCurrentText(d_func()->outerData.value<StackedTuneParamValue>());
        //        return editor;
        //    }
    }

    return nullptr;
}

QVariant TuneParamTreeItem::editorData(QWidget* editor) const
{
    switch (d_func()->outerValue.userType()) {
        case QMetaType::Float:
            return static_cast<float>(qobject_cast<const QDoubleSpinBox*>(editor)->value());
        case QMetaType::QString:
            return qobject_cast<const QLineEdit*>(editor)->text();
        case QMetaType::Int:
            if (d_func()->values.isEmpty()) {
                return qobject_cast<const QSpinBox*>(editor)->value();
            }
            else {
                return qobject_cast<const QComboBox*>(editor)->currentIndex();
            }
    }

    return QVariant();
}

void TuneParamTreeItem::resetOuterData()
{
    d_func()->outerValue = d_func()->data.value;
    d_func()->valueChanged = false;
}

void TuneParamTreeItem::setValueType(QMetaType::Type type)
{
    d_func()->data.value = d_func()->outerValue = QVariant(QVariant::Type(type));

    switch (type) {
        case QMetaType::Float:
            d_func()->min = std::numeric_limits<float>::lowest();
            d_func()->max = std::numeric_limits<float>::max();
            break;
        case QMetaType::Int:
            d_func()->min = std::numeric_limits<int>::lowest();
            d_func()->max = std::numeric_limits<int>::max();
            break;
        default:
            break;
    }
}
