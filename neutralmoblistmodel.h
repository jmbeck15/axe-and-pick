#ifndef NEUTRALMOBLISTMODEL_H
#define NEUTRALMOBLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QBitArray>

#include "listmodel.h"

// NeutralMob are stored in NeutralMobListModel
class NeutralMob : public ListItem
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

    NeutralMob(QObject * parent = 0): ListItem(parent){id_counter++;}
    explicit NeutralMob(const QString &type,
                        const float &posX,
                        const float &posY,
                        const float &posZ,
                        const float &rotation,
                        const QList<float> &unknown_floats,
                        const QBitArray &options,
                        QObject * parent = 0);

    static NeutralMob * build(QStringList & unitData);

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
    inline bool unknown_float(unsigned int index) const { return m_unknown.at(index); }
    inline bool option(unsigned int optionNumber) const { return m_options[optionNumber]; }

    // Utilities
    void print();

private:
    long m_id;

    QString m_type;
    float m_posX;
    float m_posY;
    float m_posZ;
    float m_rotation;
    QList<float> m_unknown;
    QBitArray m_options;
};

// NeutralMobListModel store NeutralMobs
class NeutralMobListModel : public ListModel
{
private:
    Q_OBJECT

public:
    NeutralMobListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = NeutralMob::TypeRole);
    Q_INVOKABLE void remove(const long id);
    Q_INVOKABLE float getFirstPosition(const char label);
    Q_INVOKABLE void add(const QString type, float x, float y, float z);
};

#endif // NEUTRALMOBLISTMODEL_H

