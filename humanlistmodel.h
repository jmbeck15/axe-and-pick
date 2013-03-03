#ifndef HUMANLISTMODEL_H
#define HUMANLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// Human are stored in HumanListModel
class Human : public ListItem
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    enum Roles {
        NameRole  = Qt::UserRole + 1,
        ProfessionRole,

        IdRole,
        FilterStringRole
    };

    Human(QObject * parent = 0): ListItem(parent){}
    explicit Human(const QString &name,
                      const QString &profession,
                      const long &id,
                      QObject * parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setProfession(QString profession);
    inline QString name() const { return m_name; }
    inline QString profession() const { return m_profession; }
    inline long id() const { return m_id; }
    QString filterString() const;

private:
    QString m_name;
    QString m_profession;
    long m_id;
};

// HumanListModel store Humans
class HumanListModel : public ListModel
{
public:
    HumanListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = Human::ProfessionRole);
};

#endif // HUMANLISTMODEL_H
