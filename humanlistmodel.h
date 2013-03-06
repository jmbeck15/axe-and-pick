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
        IdRole = Qt::UserRole + 1,
        FilterStringRole,

        ProfessionRole,
        PosXRole,
        PosYRole,
        PosZRole,
        NameRole,

        ArcherLevelRole,
        BlacksmithLevelRole,
        BuilderLevelRole,
        CarpenterLevelRole,
        EngineerLeveRole,
        FarmerLevelRole,
        FishermanLevelRole,
        ForagerLevelRole,
        InfantryLevelRole,
        MinerLevelRole,
        StoneMasonLevelRole,
        WoodChopperLevelRole,

        ExperienceRole,

        AutoChopTreesRole,
        GatherBerriesRole,
        HuntChickenRole,
        HuntBoarRole,
        ShowBowRangeRole,
        TrainNearTargetRole,

        RotationRole,

        EquipHandRole,
        EquipOffhandRole,
        EquipHeadRole,
        EquipBodyRole,
        EquipFeetRole,

        HealthRole,

        AutoequipRole,
        ReturnToCampfireRole

    };

    Human(QObject * parent = 0): ListItem(parent){}
    explicit Human(const long &id,
                   const QString &profession,
                   const float &posX,
                   const float &posY,
                   const float &posZ,
                   const QString &name,

                   const unsigned int &archerLevel,
                   const unsigned int &blacksmithLevel,
                   const unsigned int &builderLevel,
                   const unsigned int &carpenterLevel,
                   const unsigned int &engineerLevel,
                   const unsigned int &farmerLevel,
                   const unsigned int &fishermanLevel,
                   const unsigned int &foragerLevel,
                   const unsigned int &infantryLevel,
                   const unsigned int &minerLevel,
                   const unsigned int &stoneMasonLevel,
                   const unsigned int &woodChopperLevel,

                   const unsigned int &experience,

                   const bool &autoChop,
                   const bool &gatherBerries,
                   const bool &huntChicken,
                   const bool &huntBoar,
                   const bool &showBowRange,
                   const bool &trainNearTarget,

                   const float &rotation,

                   const bool &equipHand,
                   const bool &equipOffhand,
                   const bool &equipHead,
                   const bool &equipBody,
                   const bool &equipFeet,

                   const unsigned int &health,

                   const bool &autoEquip,
                   const bool &returnToCampfire,
                   QObject * parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    QString filterString() const;

    // Setters
    void setProfession(QString profession);

    // Getters
    inline long    id() const { return m_id; }

    inline QString profession() const { return m_profession; }
    inline float posX() const { return m_posX; }
    inline float posY() const { return m_posY; }
    inline float posZ() const { return m_posZ; }
    inline QString name() const { return m_name; }

    inline unsigned int archerLevel() const { return m_archerLevel; }
    inline unsigned int blacksmithLevel() const { return m_blacksmithLevel; }
    inline unsigned int builderLevel() const { return m_builderLevel; }
    inline unsigned int carpenterLevel() const { return m_carpenterLevel; }
    inline unsigned int engineerLevel() const { return m_engineerLevel; }
    inline unsigned int farmerLevel() const { return m_farmerLevel; }
    inline unsigned int fishermanLevel() const { return m_fishermanLevel; }
    inline unsigned int foragerLevel() const { return m_foragerLevel; }
    inline unsigned int infantryLevel() const { return m_infantryLevel; }
    inline unsigned int minerLevel() const { return m_minerLevel; }
    inline unsigned int stoneMasonLevel() const { return m_stoneMasonLevel; }
    inline unsigned int woodChopperLevel() const { return m_woodChopperLevel; }

    inline unsigned int experience() const { return m_experience; }

    inline bool autoChop() const { return m_autoChop; }
    inline bool gatherBerries() const { return m_gatherBerries; }
    inline bool huntChicken() const { return m_huntChicken; }
    inline bool huntBoar() const { return m_huntBoar; }
    inline bool showBowRange() const { return m_showBowRange; }
    inline bool trainNearTarget() const { return m_trainNearTarget; }

    inline float rotation() const { return m_rotation; }

    inline unsigned int equipHand() const { return m_equipHand; }
    inline unsigned int equipOffhand() const { return m_equipOffhand; }
    inline unsigned int equipHead() const { return m_equipHead; }
    inline unsigned int equipBody() const { return m_equipBody; }
    inline unsigned int equipFeet() const { return m_equipFeet; }

    inline unsigned int health() const { return m_health; }

    inline bool autoEquip() const { return m_autoEquip; }
    inline bool returnToCampfire() const { return m_returnToCampfire; }

private:
    long m_id;

    QString m_profession;
    float m_posX;
    float m_posY;
    float m_posZ;
    QString m_name;

    unsigned int m_archerLevel;
    unsigned int m_blacksmithLevel;
    unsigned int m_builderLevel;
    unsigned int m_carpenterLevel;
    unsigned int m_engineerLevel;
    unsigned int m_farmerLevel;
    unsigned int m_fishermanLevel;
    unsigned int m_foragerLevel;
    unsigned int m_infantryLevel;
    unsigned int m_minerLevel;
    unsigned int m_stoneMasonLevel;
    unsigned int m_woodChopperLevel;

    unsigned int m_experience;

    bool m_autoChop;
    bool m_gatherBerries;
    bool m_huntChicken;
    bool m_huntBoar;
    bool m_showBowRange;
    bool m_trainNearTarget;

    float m_rotation;

    unsigned int m_equipHand;
    unsigned int m_equipOffhand;
    unsigned int m_equipHead;
    unsigned int m_equipBody;
    unsigned int m_equipFeet;

    unsigned int m_health;

    bool m_autoEquip;
    bool m_returnToCampfire;
};

// HumanListModel store Humans
class HumanListModel : public ListModel
{
public:
    HumanListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = Human::ProfessionRole);
};

#endif // HUMANLISTMODEL_H
