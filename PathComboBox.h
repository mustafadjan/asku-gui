#pragma once

#include <QComboBox>

class PathComboBox : public QComboBox
{
    Q_OBJECT

public:

    explicit PathComboBox(QAbstractItemModel*, QWidget* = nullptr);

public slots:

    void updateItem(const QModelIndex&);

private slots:

    void activate(int);

signals:

    void activated(const QModelIndex&);

};
