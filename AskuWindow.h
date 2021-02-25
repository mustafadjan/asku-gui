#pragma once

#include <QWidget>

struct ElemData;

class AskuWindow : public QWidget
{
    Q_OBJECT

public:

    explicit AskuWindow(QWidget* = nullptr);

signals:

    void schemesReceived(quint32, const QHash<quint32, QJsonValue>&) const;
    void elemDatasReceived(quint32, const QHash<quint32, QHash<quint16, ElemData>>&) const;

};
