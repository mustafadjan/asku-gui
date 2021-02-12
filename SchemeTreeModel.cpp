#include "SchemeTreeModel.h"
#include "RlkTreeItem.h"
#include "ElemTreeItem.h"
#include "CtrlParamTreeItem.h"
#include "TuneParamTreeItem.h"
#include "StackedTuneParamValue.h"
#include "VoiTypes.h"
#include "Enums.h"
#include "TypesFunction.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
// todo: delete later
    #include <QTimer>
    #include <QDateTime>
    #include <QRandomGenerator>
    #include <QDebug>
#include "Loger.h"
// todo: delete later

#define sLogPrefix   "SchemeModel"

class SchemeTreeModelPrivate
{
    Q_DECLARE_PUBLIC(SchemeTreeModel)

    SchemeTreeModel* q_ptr{nullptr};

    QTimer* fakeTimerElemCtrlParam{nullptr}, *fakeTimerRlkModule{nullptr}; // todo: delete later

    struct RLKMappedItem
    {
        struct ModuleMappedItem
        {
            operator ModuleTreeItem*() const { return module; }

            ModuleTreeItem* module{nullptr};
            QHash<quint16, ElemTreeItem*> elems;
            QHash<quint32, CtrlParamTreeItem*> ctrlParams;
            QHash<quint32, TuneParamTreeItem*> tuneParams;
        };

        operator RLKTreeItem*() const { return rlk; }
        ModuleMappedItem& operator[](quint32 moduleId) { return modules[moduleId]; }
        //bool contains(quint32 moduleId) const { return modules.contains(moduleId); }
        QHash<quint32, ModuleMappedItem>::const_iterator constEnd() const {
            return modules.constEnd();
        }
        QHash<quint32, ModuleMappedItem>::const_iterator constFind(quint32 moduleId) const {
            return modules.constFind(moduleId);
        }

        RLKTreeItem* rlk{nullptr};
        QHash<quint32, ModuleMappedItem> modules;
    };

    QVector<RLKTreeItem*> rlkItems;
    QHash<quint32, RLKMappedItem> mappedItems;

    explicit SchemeTreeModelPrivate(SchemeTreeModel* q): // todo: delete later
        q_ptr(q)
    {
        mappedItems.constFind(0).value();

        setupModelData();

        fakeTimerElemCtrlParam = new QTimer(q);
        fakeTimerRlkModule = new QTimer(q);

        q_func()->connect(fakeTimerElemCtrlParam, &QTimer::timeout, q_func(), [this]
        {
            quint32 moduleId = static_cast<quint32>(EIdUser::ID_POI) << 8 | 1;
            q_func()->updateElemsData(0, moduleId, generateRandomElemData());
            q_func()->updateCtrlParamsData(0, moduleId, generateRandomCtrlParamData());
        });

        q_func()->connect(fakeTimerRlkModule, &QTimer::timeout, q_func(), [this]
        {
            q_func()->updateRlkData(generateRandomRlkModuleData(0));
            q_func()->updateModuleData(0,
                      generateRandomRlkModuleData(static_cast<quint32>(EIdUser::ID_POI) << 8 | 1));
        });

        //fakeTimerElemCtrlParam->start(5000);
        fakeTimerRlkModule->start(1000);
    }

    void setupModelData() // todo: delete later
    {
        //auto tpValue = new RangedValue<float>();
        //qDebug() << __PRETTY_FUNCTION__ << tpValue->value() << tpValue->minimum() << tpValue->maximum();
        auto rlk = new RLKTreeItem(0);
        rlkItems.append(rlk);
        mappedItems[0].rlk = rlk;
        auto elem0 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_OTS) << 8 | 0, "АЛМАЗ", rlk);
        //auto typeItem = moduleTypeItem(ModuleType::POI);
        auto elem00 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_POI) << 8 | 1, "РЛС", rlk);
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
        auto elem1 = new ModuleTreeItem(static_cast<quint32>(EIdUser::ID_POI) << 8 | 2, "ЛЭМЗ", rlk);
        auto elem10 = new ElemTreeItem(0, "Тепловизор", elem1);
        auto elem103 = new ElemTreeItem(2, "Элемент_2", elem10);
        auto tuneParam100 = new TuneParamTreeItem(0, "Тест НП 1", 5, {0, 10}, elem10);
        auto ctrlParam101 = new CtrlParamTreeItem(0, "Тест КП 1", elem10);
        auto tuneParam102 = new TuneParamTreeItem(1, "Тест НП 3", "5.6",
                                                  {"2.8", "5.6", "7.0", "9.1"}, elem10);
        auto tuneParam103 = new TuneParamTreeItem(2, "Группа НП 1", elem10);
        auto tuneParam1000 = new TuneParamTreeItem(3, "Тест НП 4", 6.1, {0.1f, 11.f}, tuneParam103);
        auto tuneParam1001 = new TuneParamTreeItem(4, "Тест НП 5", 7, {0, 12}, tuneParam103);
        auto elem11 = new ElemTreeItem(1, "СКК", elem1);
        auto tuneParam110 = new TuneParamTreeItem(3, "Тест НП 2", false, elem11);
        auto tuneParam111 = new TuneParamTreeItem(2, "Тест НП 3", true, elem11);
        auto tuneParam112 = new TuneParamTreeItem(0, "Тест НП 4", false, elem11);
        auto tuneParam113 = new TuneParamTreeItem(1, "Тест НП 5", true, elem11);
        auto elem113 = new ElemTreeItem(3, "Элемент_3", elem11);
        auto elem114 = new ElemTreeItem(4, "Элемент_4", elem113);
    }

    QHash<quint32, Pack0x24> generateRandomRlkModuleData(quint32 rlkId) const // todo: delete later
    {
        Pack0x24 data;
        data.time = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();;
        data.condition = static_cast<quint8>(QRandomGenerator::system()->
                      bounded(static_cast<int>(AbstractConditionalItem::ItemCondition::Fail) + 1));
        data.state = static_cast<quint8>(QRandomGenerator::system()->
                                      bounded(static_cast<int>(ElemTreeItem::State::TurnOff) + 1));
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
                      bounded(static_cast<int>(AbstractConditionalItem::ItemCondition::Fail) + 1));
            elemData.state = static_cast<quint8>(QRandomGenerator::system()->
                                      bounded(static_cast<int>(ElemTreeItem::State::TurnOff) + 1));
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
            ctrlParamData.time = currentTimeUtc;
            ctrlParamData.condition = static_cast<quint8>(QRandomGenerator::system()->
                 bounded(static_cast<int>(AbstractConditionalItem::ItemCondition::Uncontrol) + 1));
            ctrlParamData.value = QString::number(QRandomGenerator::system()->bounded(100));
            outData[i] = ctrlParamData;
        }

        return outData;
    }

    template<typename T1, typename T2>
    void updateItemsData(const T1&, const T2&) const;
    AbstractSchemeTreeItem* item(const QModelIndex&) const;
    QString makeModuleName(quint32) const;

};

template<typename T1, typename T2>
void SchemeTreeModelPrivate::updateItemsData(const T1& items, const T2& data) const
{
    auto itEnd = data.constEnd();
    for (auto it = data.constBegin(); it != itEnd; ++it) {
        auto itItem = items.constFind(it.key());
        //if (items.contains(it.key())) {
        //    AbstractSchemeTreeItem* item = items[it.key()];
        if (itItem != items.constEnd()) {
            AbstractSchemeTreeItem* item = itItem.value();
            auto roles = item->setData(QVariant::fromValue(it.value()));
            int row = item->parentItem() ? item->row() :
                      rlkItems.indexOf(dynamic_cast<RLKTreeItem*>(item));
            auto index = q_func()->createIndex(row, 0, item);
            if (!roles.isEmpty() && index.isValid()) {
                emit q_func()->constDataChanged(index, index, roles);
            }
        }
    }
}

AbstractSchemeTreeItem* SchemeTreeModelPrivate::item(const QModelIndex& index) const
{
    return index.isValid() ? static_cast<AbstractSchemeTreeItem*>(index.internalPointer()) : nullptr;
}

QString SchemeTreeModelPrivate::makeModuleName(quint32 id) const
{
    return QString("%1 %2").arg(idToStr(static_cast<EIdUser>(id & 0xffffff))).arg(id >> 24);
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

void SchemeTreeModel::buildScheme(quint32 rlkId, const QHash<quint32, QJsonDocument>& jsonDocuments)
{
    /*MY_LOG_DEB("qwer");
    auto it = d_func()->mappedItems.constFind(rlkId);
    if (it == d_func()->mappedItems.constEnd()) {
        auto rlkItem = new RLKTreeItem(rlkId);
        d_func()->rlkItems.append(rlkItem);
        d_func()->mappedItems[rlkId].rlk = rlkItem;
    }
    else {

    }



    auto recvElem = [] (const QJsonObject& jsonObject)
    {
        quint16 ID;
        if (jsonObject.contains("IDElem") && jsonObject["IDElem"].isDouble()) {
            int IDtmp = jsonObject["IDElem"].toInt();
            if (IDtmp)
            ID = static_cast<quint16>(jsonObject["IDElem"].toInt());
        }
        else { // элемент без номера

        }
        if (ID < 0) { // элемент без номера
            if (mElem.contains(nID)) { // один элемент без номера уже был, больше нельзя
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                            "пришло более одного элемента без номера").
                          arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                          arg(o_HeaderRecv.n_TypePack, 0, 16), ERR_COLOR);
            return;
            }
          if(oObj.contains("Elems")) {
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                              "в элемент без номера не могут входить другие элементы").
                            arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                            arg(o_HeaderRecv.n_TypePack, 0, 16), ERR_COLOR);
            return;
            }
          if(!oObj.contains("Params") && !oObj.contains("ConfigParam")) {
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                              "элемент без номера должен содержать контролируемые и/или настраиваемые параметры\n"
                              "В пришедшем сообщении элемент без номера не содержит ни контролируемых, ни настраиваемых параметров").
                              arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                              arg(o_HeaderRecv.n_TypePack, 0, 16), ERR_COLOR);
            return;
            }
        }
        else {
        if(nID < 0 || nID > 0xFFFF) {
          ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                            "неверный номер элемента = %6, возможные значения от 0 до %7").
                         arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                        .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(nID).arg(0xFFFF), ERR_COLOR);
          return;
          }
          }
        OElementSheme* pElem = new OElementSheme;
        if(!pElem) return;
        pElem->o_State.n_ID = nID;
        if(nID == -1) pElem->n_Time = -1;

        vElement.push_back(pElem);
        mElem.insert(nID, pElem);
        if(oObj.contains("Name") && oObj["Name"].isString()) pElem->s_Name = oObj["Name"].toString();
        else {
          if(nID != -1) {
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                           "отсутствует (или неверный формат) названия элемента № %6, поставлено значение Элемент_%7").
                         arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                        .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(nID).arg(nID), ERR_COLOR);
            pElem->s_Name = tr("Элемент_%1").arg(nID);
            }
          else pElem->s_Name = tr("Настройки модуля");
          }
        if(oObj.contains("ShortName") && oObj["ShortName"].isString()) pElem->s_ShortName = oObj["ShortName"].toString();
        if(oObj.contains("Params")) {
          if(oObj["Params"].isArray()) { // несколько параметров
            QJsonArray oArr = oObj["Params"].toArray();
            int nParamCnt = oArr.size();
            if(nParamCnt == 0) {
              ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                             "передано неверное количество КП в составе элемента %6 (%7): "
                              "%8 (возможные значения > 0)").
                        arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                        arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                        arg(nID).arg(nParamCnt), ERR_COLOR);
              }
            for(int i = 0; i < nParamCnt; i++) {
              if(oArr.at(i).isObject())  RecvOneParam(oArr.at(i).toObject(), pElem->v_Params, mParam);
              else
                ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                               "неверный формат данных о КП № %6 элемента %7 (%8) в составе модуля").
                          arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                          arg(o_HeaderRecv.n_TypePack, 0, 16).arg(i).arg(pElem->s_Name).
                          arg(nID), ERR_COLOR);
              }
            } // несколько параметров
          else if(oObj["Params"].isObject()) RecvOneParam(oObj["Params"].toObject(), pElem->v_Params, mParam);
          else { // ошибка
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: сообщение %4 (0x%5) "
                         "о КП элемента %6 (%7) не является форматом Json").
                       arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                      .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                      arg(nID), ERR_COLOR);
            } // ошибка
          } // есть данные о КП
        QString sCP("ConfigParam");
        if(oObj.contains(sCP)) {
          if(oObj[sCP].isArray()) { // несколько параметров
            QJsonArray oArr = oObj[sCP].toArray();
            int nParamCnt = oArr.size();
            if(nParamCnt == 0) {
              ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                             "передано неверное количество НП в составе элемента %6 (%7): "
                             "%8 (возможные значения > 0)").
                        arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                        arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                        arg(nID).arg(nParamCnt), ERR_COLOR);
              }
            for(int i = 0; i < nParamCnt; i++) {
              if(oArr.at(i).isObject())  RecvOneConfParam(oArr.at(i).toObject(), pElem->v_ConfParams, mConfParam);
              else
                ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                               "неверный формат данных о НП № %6 элемента %7 (%8) в составе модуля").
                          arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                          arg(o_HeaderRecv.n_TypePack, 0, 16).arg(i).arg(pElem->s_Name).
                          arg(nID), ERR_COLOR);
              }
            } // несколько параметров
          else if(oObj[sCP].isObject()) RecvOneConfParam(oObj[sCP].toObject(), pElem->v_ConfParams, mConfParam);
          else { // ошибка
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: сообщение %4 (0x%5) "
                         "о НП элемента %6 (%7) не является форматом Json").
                      arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack).
                      arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                      arg(nID), ERR_COLOR);
            } // ошибка
          } // есть данные о НП
        if(oObj.contains("Elements")) { // есть данные о входящих элементах
          if(oObj["Elements"].isArray()) { // содержит несколько элементов
            QJsonArray oArr = oObj["Elements"].toArray();
            int nElemCnt = oArr.size();
            if(nElemCnt == 0) {
              ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                               "передано неверное количество элементов в составе элемента %6 (%7): "
                                "%8 (возможные значения > 0)").
                           arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                          .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                          arg(nID).arg(nElemCnt), ERR_COLOR);
              }
            for(int i = 0; i < nElemCnt; i++) {
              if(oArr.at(i).isObject())  RecvOneElem(oArr.at(i).toObject(), pElem->v_Elements, mElem, mParam, mConfParam);
              else
               ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                               "неверный формат данных о элементе № %6 элемента %7 (%8) в составе модуля").
                             arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                            .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(i).arg(pElem->s_Name).
                            arg(nID), ERR_COLOR);
              }
            } // содержит несколько элементов
          else if(oObj["Elements"].isObject()) { // содержит один элемент
            RecvOneElem(oObj["Elements"].toObject(), pElem->v_Elements, mElem, mParam, mConfParam);
            }
          else { // ошибка
            ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: сообщение %4 (0x%5) "
                           "о входящих в состав элемента %6 (%7) подэлементах не является форматом Json").
                         arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                        .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(pElem->s_Name).
                        arg(nID), ERR_COLOR);
            }
          } // есть данные о входящих элементах
    };

    auto itEnd = jsonDocuments.constEnd();
    for (auto it = jsonDocuments.constBegin(); it != itEnd; ++it) {
        const auto& jsonDocument = it.value();
        if (jsonDocument.isArray()) {
            const auto jsonArray = jsonDocument.array();
            int jsonSize = jsonArray.size();

        }
    }

    QJsonDocument oJsonDocument = QJsonDocument::fromJson(QByteArray(sMess, o_HeaderRecv.n_SizeData));
    if(oJsonDocument.isNull()) {
      oJsonDocument = QJsonDocument::fromJson(QByteArray(sMess, o_HeaderRecv.n_SizeData - 1));
      if(oJsonDocument.isNull()) {
        ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: сообщение %4 (0x%5) "
                   "не является форматом Json").
               arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
              .arg(o_HeaderRecv.n_TypePack, 0, 16), ERR_COLOR);
        return;
        }
      }
    int nElemCnt = 0;
    QVector<OElementSheme*> oSheme;
    QMap<int, OElementSheme*> mElem;
    QMap<int, OParamSheme*> mParam;
    QMap<int, OConfParamSheme*> mConfParam;
    if(oJsonDocument.isArray()) { // несколько верхних элементов
      QJsonArray oArr = oJsonDocument.array();
      nElemCnt = oArr.size();
      if(nElemCnt == 0) {
        ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                     "передано неверное количество элементов в составе модуля: %6 (возможные значения > 0)").
                 arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(nElemCnt), ERR_COLOR);
        emit SigSheme(oSheme, mElem, mParam, mConfParam);
        return;
        }
      for(int i = 0; i < nElemCnt; i++) {
        if(oArr.at(i).isObject())  RecvOneElem(oArr.at(i).toObject(), oSheme, mElem, mParam, mConfParam);
        else
          ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: в сообщении %4 (0x%5) "
                       "неверный формат данных об элементе № %6 в составе модуля").
                   arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
                  .arg(o_HeaderRecv.n_TypePack, 0, 16).arg(i), ERR_COLOR);
          }
        }
    else if(oJsonDocument.isObject()) RecvOneElem(oJsonDocument.object(), oSheme, mElem, mParam, mConfParam); // один элемент в составе модуля
    else {
      ToLogFile(QString("Ошибка приема данных от модуля %1 (0x%2)_%3: сообщение %4 (0x%5) "
                   "не является форматом Json").
               arg(IDToStr(n_ID)).arg(n_ID, 0, 16).arg(n_IdxID).arg(o_HeaderRecv.n_TypePack)
              .arg(o_HeaderRecv.n_TypePack, 0, 16), ERR_COLOR);
      return;
      } */

    //auto moduleItem = d->moduleItem(moduleId);
    //int i = d->rlkItems.indexOf(moduleItem);
    //if (moduleItem) { // удаление старой схемы
    //    delete moduleItem;
    //}
    //
    //moduleItem = new ModuleTreeItem(moduleId, d->makeModuleName(moduleId));
    //if (jsonDocument.isObject()) {
    //    auto jsonObject = jsonDocument.object();
    //}
    //else if (jsonDocument.isArray()) {
    //    auto jsonArray = jsonDocument.array();
    //    for (auto jsonValue : jsonArray) {
    //        //jsonValue.
    //    }
    //}

    //d->moduleItems[moduleId] =
}

void SchemeTreeModel::updateRlkData(const QHash<quint32, Pack0x24>& data) const
{
    d_func()->updateItemsData(d_func()->mappedItems, data);
}

void SchemeTreeModel::updateModuleData(quint32 rlkId, const QHash<quint32, Pack0x24>& data) const
{
    auto it = d_func()->mappedItems.constFind(rlkId);
    if (it != d_func()->mappedItems.constEnd()) {
        d_func()->updateItemsData(it.value(), data);
    }
}

void SchemeTreeModel::updateElemsData(quint32 rlkId, quint32 moduleId,
                                      const QHash<quint16, ElemData>& data) const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        auto itModule = itRlk.value().constFind(moduleId);
        if (itModule != itRlk.value().constEnd()) {
            d_func()->updateItemsData(itModule.value().elems, data);
        }
    }
}

void SchemeTreeModel::updateCtrlParamsData(quint32 rlkId, quint32 moduleId,
                                           const QHash<quint32, CtrlParamData>& data) const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        auto itModule = itRlk.value().constFind(moduleId);
        if (itModule != itRlk.value().constEnd()) {
            d_func()->updateItemsData(itModule.value().ctrlParams, data);
        }
    }
}

void SchemeTreeModel::updateTuneParamsData(quint32 rlkId, quint32 moduleId,
                                           const QHash<quint32, QVariant>& data) const
{
    auto itRlk = d_func()->mappedItems.constFind(rlkId);
    if (itRlk != d_func()->mappedItems.constEnd()) {
        auto itModule = itRlk.value().constFind(moduleId);
        if (itModule != itRlk.value().constEnd()) {
            d_func()->updateItemsData(itModule.value().tuneParams, data);
        }
    }
}
