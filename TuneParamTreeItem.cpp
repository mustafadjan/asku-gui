#include "TuneParamTreeItem.h"
#include "TuneParamTreeItem_p.h"
#include "StackedTuneParamValue.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

TuneParamTreeItem::TuneParamTreeItem(quint32 id, const QString& name,
                                     AbstractSchemeTreeItem* parent):
    AbstractSchemeTreeItem(*new TuneParamTreeItemPrivate(ItemType::TuneParam, id, name, parent))
{
}

TuneParamTreeItem::TuneParamTreeItem(quint32 id, const QString& name, const QVariant& data,
                                     AbstractSchemeTreeItem* parent):
    TuneParamTreeItem(id, name, parent)
{
    d_func()->innerData = d_func()->outerData = data;
}

TuneParamTreeItem::TuneParamTreeItem(quint32 id, const QString& name, const QVariant& data,
                                     const QPair<float, float>& range,
                                     AbstractSchemeTreeItem* parent):
    TuneParamTreeItem(id, name, data, parent)
{
    d_func()->range = range;
}

TuneParamTreeItem::TuneParamTreeItem(quint32 id, const QString& name, const QString& data,
                                    const QSet<QString>& valueSet, AbstractSchemeTreeItem* parent):
    TuneParamTreeItem(id, name, data, parent)
{
    d_func()->valueSet = valueSet;
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
    switch (data.type()) {
        case QVariant::List:
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
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::String:
        case QVariant::Int:
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

QWidget* TuneParamTreeItem::createEditor(QWidget* parent) const
{
    switch (d_func()->outerData.type()) {
        case QVariant::Double:
            {
                auto editor = new QDoubleSpinBox(parent);
                editor->setValue(d_func()->outerData.toDouble());
                if (d_func()->range.first < d_func()->range.second) {
                    editor->setRange(static_cast<double>(d_func()->range.first),
                                     static_cast<double>(d_func()->range.second));
                }
                return editor;
            }
        case QVariant::String:
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
        case QVariant::Int:
            {
                auto editor = new QSpinBox(parent);
                editor->setValue(d_func()->outerData.toInt());
                if (d_func()->range.first < d_func()->range.second) {
                    editor->setRange(static_cast<int>(d_func()->range.first),
                                     static_cast<int>(d_func()->range.second));
                }
                return editor;
            }
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
            if (d_func()->valueSet.isEmpty()) {
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
