#pragma once

#include <QWidget>

class AskuWindow : public QWidget
{
    Q_OBJECT

public:

    explicit AskuWindow(QWidget* = nullptr);

signals:

    void modulesData(quint32, const QHash<quint32, QJsonValue>&) const;

};
