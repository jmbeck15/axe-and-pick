#ifndef SAVEDGAMELISTMODEL_H
#define SAVEDGAMELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// SavedGame are stored in SavedGameListModel
class SavedGame : public ListItem
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    enum Roles {
        NameRole  = Qt::UserRole + 1,
        DateRole,
        WorldSize,
        DayRole,
        UnitNumberRole,
        IdRole,
        FilterStringRole
    };

    SavedGame(QObject * parent = 0): ListItem(parent){}
    explicit SavedGame(const QString &gameName,
                      const QString &lastPlayed,
                      const QString &worldSize,
                      const long &day,
                      const long &unitNumber,
                      QObject * parent = 0);

    inline long id() const { return m_id; }
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setDay(long newDay);
    void setUnitNumber(long newUnitNumber);

    inline QString name() const { return m_name; }
    inline QString date() const { return m_date; }
    inline QString worldSize() const { return m_worldSize; }
    inline long day() const { return m_day; }
    inline long unitNumber() const { return m_unitNumber; }
    QString filterString() const;

private:
    QString m_name;
    QString m_date;
    QString m_worldSize;
    long m_day;
    long m_unitNumber;
    long m_id;
};

// ResourceListModels store Resources
class SavedGameListModel : public ListModel
{
public:
    SavedGameListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long identification, const QVariant &value, int role);
};

#endif // SAVEDGAMELISTMODEL_H

