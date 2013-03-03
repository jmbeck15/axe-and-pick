#include "savedgamelistmodel.h"

#include <QDebug>
#include <sstream>

SavedGame::SavedGame(const QString &gameName,
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

void SavedGame::setDay(long newDay)
{
    if(m_day != newDay)
    {
        m_day = newDay;
        emit dataChanged();
    }
}
void SavedGame::setUnitNumber(long newUnitNumber)
{
    if(m_unitNumber != newUnitNumber)
    {
        m_unitNumber = newUnitNumber;
        emit dataChanged();
    }
}

QHash<int, QByteArray> SavedGame::roleNames() const
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

QVariant SavedGame::data(int role) const
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

QString SavedGame::filterString() const
{
    std::stringstream completeString;

    completeString << name().toStdString()
                   << date().toStdString()
                   << worldSize().toStdString()
                   << day()
                   << unitNumber();
    return (QString(completeString.str().c_str()));
}

SavedGameListModel::SavedGameListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void SavedGameListModel::setData(const long identification, const QVariant &value, int role)
{
    switch (role)
    {
    case SavedGame::DayRole:
    {
        SavedGame * item = (SavedGame *)find(identification);
        item->setDay(value.toLongLong());
        break;
    }
    case SavedGame::UnitNumberRole:
    {
        SavedGame * item = (SavedGame *)find(identification);
        item->setUnitNumber(value.toLongLong());
        break;
    }
    default:
        qWarning() << "SavedGameListModel::setData does not understand what role" << role << "is.";
        break;
    }
}
