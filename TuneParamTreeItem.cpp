#include "TuneParamTreeItem.h"
#include "TuneParamTreeItem_p.h"
#include "StackedTuneParamValue.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

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
            return d_func()->innerData;
        case 2:
            return d_func()->outerData;
    }

    return QVariant();
}

QVariant TuneParamTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::ChangeTPRole:
            return QFont(QFont().family(), QFont().pointSize(),
                         d_func()->dataChanged ? QFont::Bold : QFont().weight(), QFont().italic());
    }

    return AbstractSchemeTreeItem::roleData(role);
}

QVector<int> TuneParamTreeItem::setData(const QVariant& data)
{
    QVector<int> roles;
    switch (data.userType()) {
        case QMetaType::QVariantList:
            {
                auto dataList = data.toList();
                if (dataList.size() == 2) {
                    auto time = dataList[0].toLongLong();
                    if (time) {
                        d_func()->time = time;
                        roles << Qt::TimeRole;
                    }
                    d_func()->innerData = dataList[1];
                    if (!d_func()->dataChanged) {
                        d_func()->outerData = d_func()->innerData;
                    }
                }
            }
            break;
        case QMetaType::Float:
        case QMetaType::QString:
        case QMetaType::Int:
        case QMetaType::Bool:
            qDebug() << __PRETTY_FUNCTION__ << data.type();
            d_func()->outerData = data;
            d_func()->dataChanged = true;
            roles << Qt::ChangeTPRole;
            break;
        default:
            break;
    }

    return roles;
}

bool TuneParamTreeItem::isValid(ModelType modelType) const
{
    return modelType == ModelType::TuneParamTree;
}

void TuneParamTreeItem::setType(QMetaType::Type type)
{
    if (type != QMetaType::UnknownType && d_func()->innerData.canConvert(int(type))) {
        d_func()->innerData.convert(int(type));
        d_func()->outerData.convert(int(type));
    }
    else {
        d_func()->innerData = d_func()->outerData = QVariant(QVariant::Type(type));
    }
}

void TuneParamTreeItem::setUnit(const QString& unit)
{
    d_func()->unit = unit;
}

void TuneParamTreeItem::setSaveStatus(bool b)
{
    d_func()->saveStatus = b;
}

void TuneParamTreeItem::setMin(float min)
{
    d_func()->range.first = min;
}

void TuneParamTreeItem::setMax(float max)
{
    d_func()->range.second = max;
}

void TuneParamTreeItem::setValues(const QVector<QString>& values)
{
    d_func()->values = values;
}

void TuneParamTreeItem::setDescription(const QString& description)
{
    d_func()->description = description;
}

void TuneParamTreeItem::setDefault(const QVariant& defaultValue)
{
    int type = d_func()->innerData.userType();
    if (defaultValue.canConvert(type)) {
        d_func()->defaultValue = defaultValue;
        d_func()->defaultValue.convert(type);
        d_func()->innerData = d_func()->outerData = d_func()->defaultValue;
    }
}

QWidget* TuneParamTreeItem::createEditor(QWidget* parent) const
{
    switch (d_func()->outerData.userType()) {
        case QMetaType::Float:
            {
                auto editor = new QDoubleSpinBox(parent);
                editor->setValue(d_func()->outerData.toDouble());
                if (d_func()->range.first < d_func()->range.second) {
                    editor->setRange(static_cast<double>(d_func()->range.first),
                                     static_cast<double>(d_func()->range.second));
                }
                return editor;
            }
        case QMetaType::QString:
            //if (d_func()->valueSet.isEmpty()) {
            {
                auto editor = new QLineEdit(parent);
                editor->setText(d_func()->outerData.toString());
                return editor;
            }
            //else {
            //    auto editor = new QComboBox(parent);
            //    editor->addItems(d_func()->valueSet.toList());
            //    editor->setCurrentText(d_func()->outerData.toString());
            //    return editor;
            //}
        case QMetaType::Int:
            {
                auto editor = new QSpinBox(parent);
                editor->setValue(d_func()->outerData.toInt());
                if (d_func()->range.first < d_func()->range.second) {
                    editor->setRange(static_cast<int>(d_func()->range.first),
                                     static_cast<int>(d_func()->range.second));
                }
                return editor;
            }
        case QMetaType::Bool:
        //case QVariant::UserType:
        //    if (d_func()->outerData.userType() == 1) {
        //        auto editor = new QComboBox(parent);
        //        editor->addItems(d_func()->outerData.value<StackedTuneParamValue>.toList());
        //        editor->setCurrentText(d_func()->outerData.value<StackedTuneParamValue>());
        //        return editor;
        //    }
        default:
            break;
    }

    return nullptr;
}

QVariant TuneParamTreeItem::editorData(QWidget* editor) const
{
    switch (d_func()->outerData.type()) {
        case QVariant::Double:
            return qobject_cast<const QDoubleSpinBox*>(editor)->value();
        case QVariant::String:
            if (d_func()->values.isEmpty()) {
                return qobject_cast<const QLineEdit*>(editor)->text();
            }
            else {
                return qobject_cast<const QComboBox*>(editor)->currentText();
            }
        case QVariant::Int:
            return qobject_cast<const QSpinBox*>(editor)->value();
        default:
            break;
    }

    return QVariant();
}

void TuneParamTreeItem::resetOuterData()
{
    d_func()->outerData = d_func()->innerData;
    d_func()->dataChanged = false;
}
