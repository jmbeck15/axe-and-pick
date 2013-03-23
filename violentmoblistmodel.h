#ifndef VIOLENTMOBLISTMODEL_H
#define VIOLENTMOBLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// ViolentMob are stored in ViolentMobListModel
class ViolentMob : public ListItem
{
private:
    Q_OBJECT
    Q_ENUMS(Roles)

    static long id_counter;

public:
    enum Roles {
        IdRole  = Qt::UserRole + 1,
        FilterStringRole,

        TypeRole,
        PosXRole,
        PosYRole,
        PosZRole,
        RotationRole
    };

    ViolentMob(QObject * parent = 0): ListItem(parent){id_counter++;}
    explicit ViolentMob(const QString &type,
                        const float &posX,
                        const float &posY,
                        const float &posZ,
                        const float &rotation,
                        QObject * parent = 0);


    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    QString filterString() const;

    // Setters
    void setType(QString type);

    // Getters
    inline long id() const { return m_id; }

    inline QString type() const { return m_type; }
    inline float posX() const { return m_posX; }
    inline float posY() const { return m_posY; }
    inline float posZ() const { return m_posZ; }
    inline float rotation() const { return m_rotation; }

    // Utilities
    void print();

private:
    long m_id;

    QString m_type;
    float m_posX;
    float m_posY;
    float m_posZ;
    float m_rotation;
};

// ViolentMobListModel store ViolentMobs
class ViolentMobListModel : public ListModel
{
private:
    Q_OBJECT

public:
    ViolentMobListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = ViolentMob::TypeRole);
    Q_INVOKABLE void remove(const long id);
};

#endif // VIOLENTMOBLISTMODEL_H
