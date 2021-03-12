#include "SchemeTreeModel.h"
#include "RlkTreeItem.h"
#include "ElemTreeItem.h"
#include "CtrlParamTreeItem.h"
#include "TuneParamTreeItem.h"
#include "VoiTypes.h"
#include "Enums.h"
#include "TypesFunction.h"
#include <QJsonObject>
#include <QJsonArray>
// todo: delete later
    #include <QTimer>
    #include <QDateTime>
    #include <QRandomGenerator>
    #include <QDebug>
// todo: delete later

class SchemeTreeModelPrivate
{
    Q_DECLARE_PUBLIC(SchemeTreeModel)

    SchemeTreeModel* q_ptr{nullptr};

    QTimer* fakeTimer{nullptr}, *fakeTimerRlkModule{nullptr}; // todo: delete later

    struct RLKMappedItem
    {
        struct ModuleMappedItem
        {
            ModuleMappedItem(ModuleTreeItem* module = nullptr):
                module(module)
            {
            }

            operator ModuleTreeItem*() const { return module; }
            void clear() {
                scheme = QJsonValue();
                module->removeChilds();
                elems.clear();
                ctrlParams.clear();
                tuneParams.clear();
            }

            QJsonValue scheme;
            ModuleTreeItem* module{nullptr};
            QHash<quint16, ElemTreeItem*> elems;
            QHash<quint32, CtrlParamTreeItem*> ctrlParams;
            QHash<quint32, TuneParamTreeItem*> tuneParams;
        };

        RLKMappedItem(RLKTreeItem* rlk = nullptr):
            rlk(rlk)
        {
        }

        operator RLKTreeItem*() const { return rlk; }
        ModuleMappedItem& operator[](quint32 moduleId) { return modules[moduleId]; }
        QHash<quint32, ModuleMappedItem>::const_iterator constEnd() const {
            return modules.constEnd();
        }
        QHash<quint32, ModuleMappedItem>::const_iterator constFind(quint32 moduleId) const {
            return modules.constFind(moduleId);
        }

        RLKTreeItem* rlk{nullptr};
        QHash<quint32, ModuleMappedItem> modules;
    };
    typedef RLKMappedItem::ModuleMappedItem ModuleMappedItem;

    QVector<RLKTreeItem*> rlkItems;
    QHash<quint32, RLKMappedItem> mappedItems;

    explicit SchemeTreeModelPrivate(SchemeTreeModel* q): // todo: delete later
        q_ptr(q)
    {
        //mappedItems.constFind(0).value(); // ??
        //
        //setupModelData();
        //
        //fakeTimer = new QTimer(q);
        //
        //q_func()->connect(fakeTimerElemCtrlParam, &QTimer::timeout, q_func(), [this]
        //{
        //    quint32 moduleId = static_cast<quint32>(EIdUser::ID_POI) << 8 | 1;
        //    q_func()->updateElemsData(0, moduleId, generateRandomElemData());
        //    q_func()->updateCtrlParamsData(0, moduleId, generateRandomCtrlParamData());
        //});
        //
        //q_func()->connect(fakeTimerRlkModule, &QTimer::timeout, q_func(), [this]
        //{
        //    q_func()->updateRlkData(generateRandomRlkModuleData(0));
        //    q_func()->updateModuleData(0,
        //              generateRandomRlkModuleData(static_cast<quint32>(EIdUser::ID_POI) << 8 | 1));
        //});
        //
        //q_func()->connect(fakeTimer, &QTimer::timeout, q_func(), [this]
        //{
        //    q_func()->beginResetModel();
        //    q_func()->endResetModel();
        //});
        //fakeTimer->start(10000);
    }

    void setupModelData() // todo: delete later
    {
        auto rlk = new RLKTreeItem(0);
        rlkItems.append(rlk);
        mappedItems[0].rlk = rlk;
        auto elem0 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_OTS) << 8 | 0, rlk);
        elem0->setName("АЛМАЗ");
        //auto typeItem = moduleTypeItem(ModuleType::POI);
        auto elem00 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_POI) << 8 | 1, rlk);
        elem00->setName("РЛС");
        mappedItems[0][static_cast<quint32>(EIdUser::ID_POI) << 8 | 1].module = elem00;
        auto elem004 = new ElemTreeItem(0, "Элемент_1", elem00);
        auto elem005 = new ElemTreeItem(1, "Элемент_2", elem004);
        auto elem001 = new CtrlParamTreeItem(10, "Изм. мощности ГП", elem00);
        auto ctrlParam0010 = new CtrlParamTreeItem(0, "Изм. мощность 0", elem001);
        auto ctrlParam0011 = new CtrlParamTreeItem(1, "Изм. мощность 1", elem001);
        auto ctrlParam0012 = new CtrlParamTreeItem(2, "Изм. мощность 2", elem001);
        auto ctrlParam0013 = new CtrlParamTreeItem(3, "Изм. мощность 3", elem001);
        auto ctrlParam0014 = new CtrlParamTreeItem(4, "Изм. мощность 4", elem001);
        auto ctrlParam0015 = new CtrlParamTreeItem(5, "Изм. мощность 5", elem001);
        auto elem000 = new CtrlParamTreeItem(6, "Изм.", elem00);
        auto elem002 = new CtrlParamTreeItem(11, "Изм. мощности ВП", elem00);
        auto ctrlParam0020 = new CtrlParamTreeItem(7, "Изм. мощность 6", elem002);
        auto ctrlParam0021 = new CtrlParamTreeItem(8, "Изм. мощность 7", elem002);
        auto elem003 = new CtrlParamTreeItem(9, "Изм. мощность 8", elem00);
        auto elem1 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_POI) << 8 | 2, rlk);
        elem1->setName("ЛЭМЗ");
        auto elem10 = new ElemTreeItem(0, "Тепловизор", elem1);
        auto elem103 = new ElemTreeItem(2, "Элемент_2", elem10);
        auto tuneParam100 = new TuneParamTreeItem(0, "Тест НП 1", elem10);
        tuneParam100->setType(Bool);
        //tuneParam100->setMin(1.f);
        //tuneParam100->setMax(10.f);
        qint8 b = false;
        tuneParam100->setData(QVariant::fromValue(TuneParamData{0, QByteArray(reinterpret_cast<const char*>(&b), sizeof(b))}));
        auto ctrlParam101 = new CtrlParamTreeItem(0, "Тест КП 1", elem10);
        auto tuneParam102 = new TuneParamTreeItem(1, "Тест НП 3", elem10);
        tuneParam102->setType(Int32);
        tuneParam102->setMin(0);
        int i = 10;
        tuneParam102->setData(QVariant::fromValue(TuneParamData{0, QByteArray(reinterpret_cast<const char*>(&i), sizeof(i))}));
        auto tuneParam103 = new TuneParamTreeItem(0, "Группа НП 1", elem10);
        auto tuneParam1000 = new TuneParamTreeItem(2, "Тест НП 4", tuneParam103);
        tuneParam1000->setType(String);
        QString str("Текст text");
        tuneParam1000->setData(QVariant::fromValue(TuneParamData{0, str.toUtf8()}));
        auto tuneParam1001 = new TuneParamTreeItem(3, "Тест НП 5", tuneParam103);
        tuneParam1001->setType(Int32);
        tuneParam1001->setValues({"Вкл.", "Выкл.", "Перезагр."});
        i = 1;
        tuneParam1001->setData(QVariant::fromValue(TuneParamData{0, QByteArray(reinterpret_cast<const char*>(&i), sizeof(i))}));
        auto elem11 = new ElemTreeItem(1, "СКК", elem1);
        auto tuneParam110 = new TuneParamTreeItem(4, "Тест НП 2", elem11);
        auto tuneParam111 = new TuneParamTreeItem(5, "Тест НП 3", elem11);
        auto tuneParam112 = new TuneParamTreeItem(6, "Тест НП 4", elem11);
        auto tuneParam113 = new TuneParamTreeItem(7, "Тест НП 5", elem11);
        auto elem113 = new ElemTreeItem(3, "Элемент_3", elem11);
        auto elem114 = new ElemTreeItem(4, "Элемент_4", elem113);
    }

    QHash<quint32, Pack0x24> generateRandomRlkModuleData(quint32 rlkId) const // todo: delete later
    {
        Pack0x24 data;
        data.time = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();;
        data.condition = static_cast<quint8>(QRandomGenerator::system()->bounded(eStError + 1));
        data.state = static_cast<quint8>(QRandomGenerator::system()->bounded(eWkOffing + 1));
        data.RGDV = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.RAF = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.local = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.imit = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.hasTP = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.TP = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.hasTP = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.TP = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.mode = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
        data.numVOI = static_cast<quint8>(QRandomGenerator::system()->bounded(1000));

        QHash<quint32, Pack0x24> outData;
        outData[rlkId] = data;

        return outData;
    }

    QHash<quint16, ElemData> generateRandomElemData() const // todo: delete later
    {
        QHash<quint16, ElemData> outData;

        auto currentTimeUtc = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

        for (quint16 i = 0; i < 2; ++i) {
            ElemData elemData;
            elemData.time = currentTimeUtc;
            elemData.condition = static_cast<quint8>(QRandomGenerator::system()->
                                                     bounded(eStError + 1));
            elemData.state = static_cast<quint8>(QRandomGenerator::system()->
                                                 bounded(eWkOffing + 1));
            elemData.local = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
            elemData.imit = static_cast<quint8>(QRandomGenerator::system()->bounded(2));
            outData[i] = elemData;
        }

        return outData;
    }

    QHash<quint32, CtrlParamData> generateRandomCtrlParamData() const // todo: delete later
    {
        QHash<quint32, CtrlParamData> outData;

        auto currentTimeUtc = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

        for (quint32 i = 0; i < 10; ++i) {
            CtrlParamData ctrlParamData;
            ctrlParamData.timeRecv = currentTimeUtc;
            ctrlParamData.condition = static_cast<quint8>(QRandomGenerator::system()->
                                                          bounded(eStNoControl + 1));
            ctrlParamData.value = QString::number(QRandomGenerator::system()->bounded(100));
            outData[i] = ctrlParamData;
        }

        return outData;
    }

    template <typename T>
    void updateItemData(AbstractSchemeTreeItem*, const T&) const;
    template<typename T1, typename T2>
    void updateItemsData(const T1&, const T2&) const;
    AbstractSchemeTreeItem* item(const QModelIndex&) const;
    QModelIndex index(AbstractSchemeTreeItem*) const;

    QHash<quint32, RLKMappedItem>::iterator insertRlk(quint32);
    QHash<quint32, ModuleMappedItem>::iterator insertModule(quint32, RLKMappedItem&);

    template<typename T1, typename T2>
    void recvValue(const QJsonValue&, T1&, T2*,
                   void (SchemeTreeModelPrivate::*)(const QJsonObject&, T1&, T2*));
    void recvElem(const QJsonObject&, ModuleMappedItem&, AbstractElemTreeItem*);
    void recvCtrlParam(const QJsonObject&, QHash<quint32, CtrlParamTreeItem*>&,
                       AbstractSchemeTreeItem*);
    void recvTuneParam(const QJsonObject&, QHash<quint32, TuneParamTreeItem*>&,
                       AbstractSchemeTreeItem*);

};

template <typename T>
void SchemeTreeModelPrivate::updateItemData(AbstractSchemeTreeItem* item, const T& data) const
{
    auto roles = item->setData(QVariant::fromValue(data));
    if (!roles.isEmpty()) {
        auto index = this->index(item);
        emit q_func()->constDataChanged(index, index, roles);
    }
}

template<typename T1, typename T2>
void SchemeTreeModelPrivate::updateItemsData(const T1& items, const T2& data) const
{
    auto itEnd = data.constEnd();
    for (auto it = data.constBegin(); it != itEnd; ++it) {
        auto itItem = items.constFind(it.key());
        if (itItem != items.constEnd()) {
            updateItemData(*itItem, *it);
        }
    }
}

AbstractSchemeTreeItem* SchemeTreeModelPrivate::item(const QModelIndex& index) const
{
    return index.isValid() ? static_cast<AbstractSchemeTreeItem*>(index.internalPointer()) : nullptr;
}

QModelIndex SchemeTreeModelPrivate::index(AbstractSchemeTreeItem* item) const
{
    int row = item->parentItem() ? item->row() : rlkItems.indexOf(dynamic_cast<RLKTreeItem*>(item));
    return q_func()->createIndex(row, 0, item);
}

QHash<quint32, SchemeTreeModelPrivate::RLKMappedItem>::iterator
SchemeTreeModelPrivate::insertRlk(quint32 rlkId)
{
    int size = q_func()->rowCount();
    q_func()->beginInsertRows(QModelIndex(), size, size);
    auto rlkItem = new RLKTreeItem(rlkId);
    rlkItems.append(rlkItem);
    auto it = mappedItems.insert(rlkId, {rlkItem});
    q_func()->endInsertRows();
    return it;
}

QHash<quint32, SchemeTreeModelPrivate::ModuleMappedItem>::iterator
SchemeTreeModelPrivate::insertModule(quint32 moduleId, RLKMappedItem& mappedItem)
{
    auto index = this->index(mappedItem);
    int size = q_func()->rowCount(index);
    q_func()->beginInsertRows(index, size, size);
    auto moduleItem = new ModuleTreeItem(moduleId, mappedItem);
    auto it = mappedItem.modules.insert(moduleId, {moduleItem});
    q_func()->endInsertRows();
    return it;
}

template<typename T1, typename T2>
void SchemeTreeModelPrivate::recvValue(const QJsonValue& jsonValue, T1& items, T2* parentItem,
                                void (SchemeTreeModelPrivate::*recv)(const QJsonObject&, T1&, T2*))
{
    if (jsonValue.isArray()) {
        for (const auto jsonValueRef : jsonValue.toArray()) {
            (this->*recv)(jsonValueRef.toObject(), items, parentItem);
        }
    }
    else if (jsonValue.isObject()) {
        (this->*recv)(jsonValue.toObject(), items, parentItem);
    }
}

void SchemeTreeModelPrivate::recvElem(const QJsonObject& jsonObject,
                                      ModuleMappedItem& moduleMappedItem,
                                      AbstractElemTreeItem* parent)
{
    AbstractElemTreeItem* item = nullptr;
    const auto& jsonIDElem = jsonObject["IDElem"];
    if (jsonIDElem.isDouble()) {
        quint16 id = static_cast<quint16>(jsonIDElem.toInt());
        item = moduleMappedItem.elems[id] =
        new ElemTreeItem(id, jsonObject["Name"].toString(), parent);
    }
    else { // элемент без номера
        item = parent;
    }

    recvValue(jsonObject["Elements"], moduleMappedItem, item, &SchemeTreeModelPrivate::recvElem);

    recvValue(jsonObject["Params"], moduleMappedItem.ctrlParams,
              static_cast<AbstractSchemeTreeItem*>(item), &SchemeTreeModelPrivate::recvCtrlParam);

    recvValue(jsonObject["ConfigParam"], moduleMappedItem.tuneParams,
              static_cast<AbstractSchemeTreeItem*>(item), &SchemeTreeModelPrivate::recvTuneParam);
}

void SchemeTreeModelPrivate::recvCtrlParam(const QJsonObject& jsonObject,
                                           QHash<quint32, CtrlParamTreeItem*>& ctrlParamItems,
                                           AbstractSchemeTreeItem* parent)
{
    quint32 id = static_cast<quint32>(jsonObject["IDParam"].toDouble());
    auto item = new CtrlParamTreeItem(id, jsonObject["Name"].toString(), parent);
    item->setDescription(jsonObject["Description"].toString());

    if (jsonObject.contains("Params")) { // группа КП
        item->setType(Group);

        recvValue(jsonObject["Params"], ctrlParamItems, static_cast<AbstractSchemeTreeItem*>(item),
                  &SchemeTreeModelPrivate::recvCtrlParam);
    }
    else {
        ctrlParamItems[id] = item;
        item->setType(static_cast<ParamType>(jsonObject["Type"].toInt(-1)));
        item->setFormat(jsonObject["Format"].toString());
        QHash<int, QString> templates;
        auto recvTemplate = [&templates] (const QJsonObject& jsonTemplate) {
            const auto& jsonValue = jsonTemplate["Value"];
            if (jsonValue.isDouble()) {
                templates[jsonValue.toInt()] = jsonTemplate["Text"].toString();
            }
            else if (jsonValue.isBool()) {
                templates[jsonValue.toBool()] = jsonTemplate["Text"].toString();
            }
        };
        const auto& jsonTemplates = jsonObject["Template"];
        if (jsonTemplates.isArray()) {
            for (const auto jsonTemplate : jsonTemplates.toArray()) {
                recvTemplate(jsonTemplate.toObject());
            }
        }
        else if (jsonTemplates.isObject()) {
            recvTemplate(jsonTemplates.toObject());
        }
        item->setTemplates(templates);
    }
}

void SchemeTreeModelPrivate::recvTuneParam(const QJsonObject& jsonObject,
                                           QHash<quint32, TuneParamTreeItem*>& tuneParamItems,
                                           AbstractSchemeTreeItem* parent)
{
    quint32 id = static_cast<quint32>(jsonObject["IDConfigParam"].toDouble(0.));
    auto item = new TuneParamTreeItem(id, jsonObject["NameConfig"].toString(), parent);
    item->setDescription(jsonObject["Description"].toString());

    if (jsonObject.contains("ConfigParam")) { // группа НП
        item->setType(Group);

        recvValue(jsonObject["ConfigParam"], tuneParamItems,
                  static_cast<AbstractSchemeTreeItem*>(item),
                  &SchemeTreeModelPrivate::recvTuneParam);
    }
    else {
        item = tuneParamItems[id] = item;
        if (jsonObject.contains("Values")) {
            item->setType(Int32);
            const auto& jsonValues = jsonObject["Values"];
            QStringList values;
            if (jsonValues.isArray()) {
                for (const auto jsonValueRef : jsonValues.toArray()) {
                    values.append(jsonValueRef.toString());
                }
            }
            else if (jsonValues.isString()) {
                values.append(jsonValues.toString());
            }
            item->setValues(values);
        }
        else {
            item->setType(static_cast<ParamType>(jsonObject["TypeConfig"].toInt(-1)));
            if (jsonObject.contains("RangeMin")) {
                item->setMin(static_cast<float>(jsonObject["RangeMin"].toDouble()));
            }
            if (jsonObject.contains("RangeMax")) {
                item->setMax(static_cast<float>(jsonObject["RangeMax"].toDouble()));
            }
        }
        item->setSaveProp(jsonObject["IsSave"].toBool());
    }
}

SchemeTreeModel::SchemeTreeModel(QObject* parent):
    QAbstractItemModel(parent),
    d(new SchemeTreeModelPrivate(this))
{
    //d_func()->q_ptr = this; // todo: uncomment after deletion of debug methods (in private class)

    connect(this, &SchemeTreeModel::constDataChanged, this, &QAbstractItemModel::dataChanged);
}

SchemeTreeModel::~SchemeTreeModel()
{
    // todo: delete items by one of two ways (containers)
    delete d;
}

QModelIndex SchemeTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (hasIndex(row, column, parent)) {
        auto parentItem = d_func()->item(parent);
        auto childItem = parentItem ? parentItem->child(row) : d_func()->rlkItems.value(row);
        if (childItem) {
            return createIndex(row, column, childItem);
        }
    }

    return QModelIndex();
}

QModelIndex SchemeTreeModel::parent(const QModelIndex& child) const
{
    if (child.isValid()) {
        auto childItem = d_func()->item(child);
        auto parentItem = childItem->parentItem();
        if (parentItem) {
            int row = parentItem->parentItem() ? parentItem->row() :
                      d_func()->rlkItems.indexOf(dynamic_cast<RLKTreeItem*>(parentItem));
            return createIndex(row, 0, parentItem);
        }
    }

    return QModelIndex();
}

int SchemeTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() <= 0) {
        auto parentItem = d_func()->item(parent);
        if (parentItem) {
            return parentItem->childCount();
        }
        else {
            return d_func()->rlkItems.size();
        }
    }

    return 0;
}

int SchemeTreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return d_func()->item(parent)->columnCount();
    }

    return 1;
}

QVariant SchemeTreeModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid()) {
        auto item = d_func()->item(index);
        switch (role) {
            case Qt::DisplayRole:
                return item->data(index.column());
            case Qt::FullPathRole:
            case Qt::IDRole:
            case Qt::IsModuleRole:
                return item->roleData(role);
        }
    }

    return QVariant();
}

void SchemeTreeModel::buildSchemes(quint32 rlkId, const QHash<quint32, QJsonValue>& jsonSchemes)
{
    auto it = d_func()->mappedItems.find(rlkId);
    if (it == d_func()->mappedItems.end()) {
        it = d_func()->insertRlk(rlkId);
    }
    auto& rlkMappedItem = it.value();

    auto itEnd = jsonSchemes.constEnd();
    for (auto schemeIt = jsonSchemes.constBegin(); schemeIt != itEnd; ++schemeIt) {
        quint32 moduleId = schemeIt.key();

        auto it = rlkMappedItem.modules.find(moduleId);
        if (it == rlkMappedItem.modules.end()) {
            it = d_func()->insertModule(moduleId, rlkMappedItem);
        }
        auto& moduleMappedItem = it.value();

        const auto& moduleScheme = schemeIt.value();
        if (moduleMappedItem.scheme != moduleScheme) { // схема изменилась
            // ресетит всю модель, то есть все РЛК и их модули, и сворачивается все дерево
            // todo: поменять на deleteRows/insertRows индивидуально и соблюдая порядок
            beginResetModel();
            moduleMappedItem.clear();
            moduleMappedItem.scheme = moduleScheme;
            d_func()->recvValue(schemeIt.value(), moduleMappedItem,
                                static_cast<AbstractElemTreeItem*>(moduleMappedItem.module),
                                &SchemeTreeModelPrivate::recvElem);
            endResetModel();
        }
    }
}

void SchemeTreeModel::updateRlk(quint32 rlkId, const Pack0x24& data) const
{
    auto it = d_func()->mappedItems.constFind(rlkId);
    if (it != d_func()->mappedItems.constEnd()) {
        d_func()->updateItemData(*it, data);
    }
}

void SchemeTreeModel::updateModules(quint32 rlkId, const QHash<quint32, Pack0x24>& data) const
{
    auto it = d_func()->mappedItems.constFind(rlkId);
    if (it != d_func()->mappedItems.constEnd()) {
        d_func()->updateItemsData(*it, data);
    }
}

void SchemeTreeModel::updateElems(quint32 rlkId,
                                  const QHash<quint32, QHash<quint16, ElemData>>& data) const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        const auto& rlkMappedItem = itRlk.value();
        auto itModuleEnd = rlkMappedItem.constEnd();
        auto itEnd = data.constEnd();
        for (auto it = data.constBegin(); it != itEnd; ++it) {
            auto itModule = rlkMappedItem.constFind(it.key());
            if (itModule != itModuleEnd) {
                d_func()->updateItemsData(itModule->elems, *it);
            }
        }
    }
}

void SchemeTreeModel::updateCtrlParams(quint32 rlkId,
                                       const QHash<quint32, QHash<quint32, CtrlParamData>>& data)
const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        const auto& rlkMappedItem = itRlk.value();
        auto itModuleEnd = rlkMappedItem.constEnd();
        auto itEnd = data.constEnd();
        for (auto it = data.constBegin(); it != itEnd; ++it) {
            auto itModule = rlkMappedItem.constFind(it.key());
            if (itModule != itModuleEnd) {
                d_func()->updateItemsData(itModule->ctrlParams, *it);
            }
        }
    }
}

void SchemeTreeModel::updateTuneParams(quint32 rlkId,
                                       const QHash<quint32, QHash<quint32, TuneParamData>>& data)
const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        const auto& rlkMappedItem = itRlk.value();
        auto itModuleEnd = rlkMappedItem.constEnd();
        auto itEnd = data.constEnd();
        for (auto it = data.constBegin(); it != itEnd; ++it) {
            auto itModule = rlkMappedItem.constFind(it.key());
            if (itModule != itModuleEnd) {
                d_func()->updateItemsData(itModule->tuneParams, *it);
            }
        }
    }
}
