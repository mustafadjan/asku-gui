#pragma once

#include <QLineEdit>

class QAbstractItemModel;
class PathCompleter;

class PathLineEdit : public QLineEdit
{
    Q_OBJECT

public:

    explicit PathLineEdit(QAbstractItemModel*, QWidget* = nullptr);

    QModelIndex mapFromSource(const QModelIndex&) const;
    QString fullPath(const QModelIndex&) const;

protected:

    void mouseReleaseEvent(QMouseEvent*) override;

signals:

    void activated(const QModelIndex&);

};
