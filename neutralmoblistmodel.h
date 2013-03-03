#ifndef NEUTRALMOBLISTMODEL_H
#define NEUTRALMOBLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// NeutralMob are stored in NeutralMobListModel
class NeutralMob : public ListItem
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    enum Roles {
        TypeRole  = Qt::UserRole + 1,

        IdRole,
        FilterStringRole
    };

    NeutralMob(QObject * parent = 0): ListItem(parent){}
    explicit NeutralMob(const QString &type,
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

// NeutralMobListModel store NeutralMobs
class NeutralMobListModel : public ListModel
{
public:
    NeutralMobListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = NeutralMob::TypeRole);
};

#endif // NEUTRALMOBLISTMODEL_H

