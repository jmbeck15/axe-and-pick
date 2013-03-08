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
        IdRole  = Qt::UserRole + 1,
        FilterStringRole,

        TypeRole,
        PosXRole,
        PosYRole,
        PosZRole,
        RotationRole
    };

    NeutralMob(QObject * parent = 0): ListItem(parent){}
    explicit NeutralMob(const long &id,
                        const QString &type,
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

// NeutralMobListModel store NeutralMobs
class NeutralMobListModel : public ListModel
{
public:
    NeutralMobListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = NeutralMob::TypeRole);
};

#endif // NEUTRALMOBLISTMODEL_H

