#include "AbstractElemTreeItem.h"
#include "AbstractElemTreeItem_p.h"
#include <QPainter>

const QMap<AbstractElemTreeItem::State, QImage> populateStateImages()
{
    QImage image(8, 8, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    QRect rect(image.rect() - QMargins(0, 0, 1, 1));
    auto getRound = [&painter, rect] (const QColor& color) -> QImage
    {
        painter.setBrush(QBrush(color));
        painter.drawEllipse(rect);
        return *dynamic_cast<QImage*>(painter.device());
    };

    QMap<AbstractElemTreeItem::State, QImage> stateImages
    {
        {AbstractElemTreeItem::State::On, getRound(Qt::green)},
        {AbstractElemTreeItem::State::Off, getRound(Qt::red)},
        {AbstractElemTreeItem::State::TurnOn, getRound(Qt::yellow)},
        {AbstractElemTreeItem::State::TurnOff, getRound(qRgb(255, 127, 0))}
    };

    return stateImages;
}

static const QMap<AbstractElemTreeItem::State, QImage> stateImages(populateStateImages());

AbstractElemTreeItem::AbstractElemTreeItem(AbstractElemTreeItemPrivate& d):
    AbstractSchemeTreeItem(d)
{
}

int AbstractElemTreeItem::columnCount() const
{
    return 4;
}

QVariant AbstractElemTreeItem::data(int column) const
{
    switch (column) {
        case 0:
            return d_func()->name;
    }

    return QVariant();
}

QVariant AbstractElemTreeItem::roleData(int role) const
{
    switch (role) {
        case Qt::ConditionRole:
            return conditionBrush();
        case Qt::StateRole:
            return stateImages[static_cast<State>(d_func()->data.state)];
        case Qt::ElemFlagsRole:
            QFont font;
            font.setItalic(d_func()->data.imit);
            QFontMetrics fontMetrics(font);
            QString name = d_func()->name;
            QPixmap pixmap(fontMetrics.horizontalAdvance(name) + 12, fontMetrics.height() + 12);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::SolidLine);
            painter.setFont(font);
            QRect rect = pixmap.rect();
            QPainterPath painterPath;
            painterPath.addRoundedRect(rect, 9, 9);
            painter.fillPath(painterPath, conditionBrush());
            if (d_func()->data.local) {
                rect -= {3, 3, 3, 3};
                painterPath.addRoundedRect(rect, 9, 9);
                painter.fillPath(painterPath, Qt::yellow);
            }
            painter.drawPath(painterPath);
            painter.drawText(rect, Qt::AlignCenter, name);
            return pixmap;
    }

    return AbstractSchemeTreeItem::roleData(role);
}

bool AbstractElemTreeItem::isValid(ModelType modelType) const
{
    switch (modelType) {
        case ModelType::ElemTree:
        case ModelType::ElemList:
            return true;
        case ModelType::CtrlParamTable:
            return d_func()->hasChild(ItemType::CtrlParam, true);
        case ModelType::TuneParamTree:
            return d_func()->hasChild(ItemType::TuneParam, false);
        default:
            return false;
    }
}

AbstractConditionalItem::ItemCondition AbstractElemTreeItem::condition() const
{
    return static_cast<ItemCondition>(d_func()->data.condition);
}

AbstractElemTreeItemPrivate::AbstractElemTreeItemPrivate(quint32 id, const QString& name,
                                                         AbstractElemTreeItem* parent):
    AbstractSchemeTreeItemPrivate(id, name, parent)
{
    data.condition = static_cast<quint8>(AbstractConditionalItem::ItemCondition::Unknown);
    data.state = static_cast<quint8>(AbstractElemTreeItem::State::Off);
    data.local = data.imit = false;
}

bool AbstractElemTreeItemPrivate::hasChild(ItemType itemType, bool checkHasntSelfChild) const
{
    for (const auto childItem : childItems) {
        auto item = dynamic_cast<AbstractSchemeTreeItem*>(childItem);
        if (item->type() == itemType && (checkHasntSelfChild ? !item->hasChild() : true)) {
            return true;
        }
    }

    return false;
}
