#include "PathLineEdit.h"
#include "PathCompleter.h"
#include <QMouseEvent>

PathLineEdit::PathLineEdit(QAbstractItemModel* model, QWidget* parent):
    QLineEdit(parent)
{
    setReadOnly(true);
    auto completer = new PathCompleter(model, this);
    completer->setWidget(this); // todo: перенести в PathCompleter
    connect(completer, &PathCompleter::activated, this, &PathLineEdit::activated);
    setCompleter(completer);
}

QModelIndex PathLineEdit::mapFromSource(const QModelIndex& sourceIndex) const
{
    return qobject_cast<PathCompleter*>(QLineEdit::completer())->mapFromSource(sourceIndex);
}

QString PathLineEdit::fullPath(const QModelIndex& index) const
{
    return completer()->pathFromIndex(index);
}

void PathLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QString currentPath = text();
        int pos = currentPath.indexOf('/', cursorPositionAt(event->pos()));
        completer()->setCompletionPrefix(currentPath.left(pos) + '/');
        completer()->complete();
    }

    QLineEdit::mouseReleaseEvent(event);
}
