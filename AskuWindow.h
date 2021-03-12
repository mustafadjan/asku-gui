#pragma once

#include <QWidget>

struct ElemData;
struct CtrlParamData;
struct Pack0x24;
struct TuneParamData;
struct OneConfigParamValue;

class AskuWindow : public QWidget
{
    Q_OBJECT

public:

    explicit AskuWindow(QWidget* = nullptr);

signals:

    void schemesRecvd(quint32, const QHash<quint32, QJsonValue>&);
    void elemsRecvd(quint32, const QHash<quint32, QHash<quint16, ElemData>>&);
    void ctrlParamsRecvd(quint32, const QHash<quint32, QHash<quint32, CtrlParamData>>&);
    void moduleStatsRecvd(quint32, const QHash<quint32, Pack0x24>&);
    void tuneParamsRecvd(quint32, const QHash<quint32, QHash<quint32, TuneParamData>>&);

    void newTuneParamValues(quint32, quint32, qint64, const QList<OneConfigParamValue>&);

};
