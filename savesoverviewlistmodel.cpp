#include "savesoverviewlistmodel.h"

#include <QDebug>
#include <sstream>

SavesOverviewItem::SavesOverviewItem(const QString &gameName,
                   const QString &lastPlayed,
                   const QString &worldSize,
                   const long &day,
                   const long &unitNumber,
                   QObject * parent)
    : ListItem(parent),
      m_name(gameName),
      m_date(lastPlayed),
      m_worldSize(worldSize),
      m_day(day),
      m_unitNumber(unitNumber)
{
}

void SavesOverviewItem::setDay(long newDay)
{
    if(m_day != newDay)
    {
        m_day = newDay;
        emit dataChanged();
    }
}
void SavesOverviewItem::setUnitNumber(long newUnitNumber)
{
    if(m_unitNumber != newUnitNumber)
    {
        m_unitNumber = newUnitNumber;
        emit dataChanged();
    }
}

QHash<int, QByteArray> SavesOverviewItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[DateRole] = "date";
    names[WorldSize] = "worldSize";
    names[DayRole] = "day";
    names[UnitNumberRole] = "unitNumber";
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant SavesOverviewItem::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case DateRole:
        return date();
    case WorldSize:
        return worldSize();
    case DayRole:
        return (int)day();
    case UnitNumberRole:
        return (int)unitNumber();
    case IdRole:
        return (int)id();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString SavesOverviewItem::filterString() const
{
    std::stringstream completeString;

    completeString << name().toStdString()
                   << date().toStdString()
                   << worldSize().toStdString()
                   << day()
                   << unitNumber();
    return (QString(completeString.str().c_str()));
}

SavesOverviewListModel::SavesOverviewListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void SavesOverviewListModel::setData(const long identification, const QVariant &value, int role)
{
    switch (role)
    {
    case SavesOverviewItem::DayRole:
    {
        SavesOverviewItem * item = (SavesOverviewItem *)find(identification);
        item->setDay(value.toLongLong());
        break;
    }
    case SavesOverviewItem::UnitNumberRole:
    {
        SavesOverviewItem * item = (SavesOverviewItem *)find(identification);
        item->setUnitNumber(value.toLongLong());
        break;
    }
    default:
        qWarning() << "SavesOverviewListModel::setData does not understand what role" << role << "is.";
        break;
    }
}
