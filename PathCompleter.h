#pragma once

#include <QCompleter>

class PathCompleter : public QCompleter
{
    Q_OBJECT

public:

    explicit PathCompleter(QAbstractItemModel*, QObject* = nullptr);

    QModelIndex mapFromSource(const QModelIndex&) const;

    QString pathFromIndex(const QModelIndex&) const override;
    QStringList splitPath(const QString&) const override;

protected:

    bool eventFilter(QObject*, QEvent*) override;

private slots:

    void convertToSource(const QModelIndex&);

signals:

    void activated(const QModelIndex&);

};
