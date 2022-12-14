#include "AbstractElemTreeItem.h"
#include "AbstractElemTreeItem_p.h"
#include "Enums.h"
#include <QPainter>

const QMap<EModulWork, QImage> populateStateImages()
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

    QMap<EModulWork, QImage> stateImages
    {
        {eWkOff,    getRound(Qt::red)},
        {eWkOn,     getRound(Qt::green)},
        {eWkOning,  getRound(Qt::yellow)},
        {eWkOffing, getRound(qRgb(255, 127, 0))}
    };

    return stateImages;
}

static const QMap<EModulWork, QImage> stateImages(populateStateImages());

AbstractElemTreeItem::AbstractElemTreeItem(AbstractElemTreeItemPrivate& d):
    AbstractSchemeTreeItem(d)
{
}

int AbstractElemTreeItem::columnCount() const
{
    return 5; // т.к. детьми могут быть КП, а представление спрашивает количество столбцов именно у
              // родительского индекса, можно было бы считать динамически (в зависимости от типа
              // детей), но не имеет особого смысла, в нужных местах представления сами регулируют
              // количество столбцов, а это просто "верхний порог"
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
            return stateImages[static_cast<EModulWork>(d_func()->data.state)];
        // pixmap модуля/элемента с состоянием и признаками имитации и местн.
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

EModulState AbstractElemTreeItem::condition() const
{
    return static_cast<EModulState>(d_func()->data.condition);
}

AbstractElemTreeItemPrivate::AbstractElemTreeItemPrivate(quint32 id, const QString& name,
                                                         AbstractElemTreeItem* parent):
    AbstractSchemeTreeItemPrivate(id, name, parent)
{
    data.condition = eStUnknown;
    data.state = eWkOff;
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
