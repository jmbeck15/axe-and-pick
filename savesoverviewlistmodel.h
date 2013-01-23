#ifndef SAVESOVERVIEWLISTMODEL_H
#define SAVESOVERVIEWLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// SavesOverviewItems are stored in SavesOverviewListModels
class SavesOverviewItem : public ListItem
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

    SavesOverviewItem(QObject * parent = 0): ListItem(parent){}
    explicit SavesOverviewItem(const QString &gameName,
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
class SavesOverviewListModel : public ListModel
{
public:
    SavesOverviewListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long identification, const QVariant &value, int role);
};

#endif // SAVESOVERVIEWLISTMODEL_H
