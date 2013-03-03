#ifndef VIOLENTMOBLISTMODEL_H
#define VIOLENTMOBLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// ViolentMob are stored in ViolentMobListModel
class ViolentMob : public ListItem
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    enum Roles {
        TypeRole  = Qt::UserRole + 1,

        IdRole,
        FilterStringRole
    };

    ViolentMob(QObject * parent = 0): ListItem(parent){}
    explicit ViolentMob(const QString &type,
                        const long &id,
                        QObject * parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setType(QString type);
    inline QString type() const { return m_type; }
    inline long id() const { return m_id; }
    QString filterString() const;

private:
    QString m_type;
    long m_id;
};

// ViolentMobListModel store ViolentMobs
class ViolentMobListModel : public ListModel
{
public:
    ViolentMobListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = ViolentMob::TypeRole);
};

#endif // VIOLENTMOBLISTMODEL_H

