#ifndef HUMANLISTMODEL_H
#define HUMANLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QBitArray>

#include "listmodel.h"

// Human are stored in HumanListModel
class Human : public ListItem
{
private:
    Q_OBJECT
    Q_ENUMS(Roles)

    static long id_counter;
    static QString m_unitVersion;

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
        TailorLevelRole,
        UnknownUnit1LevelRole,
        UnknownUnit2LevelRole,

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

        HealthRole

        // NOTE: Options and unknown quantities are yet to be added.
    };

    Human(QObject * parent = 0): ListItem(parent){id_counter++;}

    explicit Human(const QString &profession,
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
                   const unsigned int &tailorLevel,
                   const unsigned int &unknownUnit1Level,
                   const unsigned int &unknownUnit2Level,

                   const unsigned int &experience,

                   const bool &autoChop,
                   const bool &gatherBerries,
                   const bool &huntChicken,
                   const bool &huntBoar,
                   const bool &showBowRange,
                   const bool &trainNearTarget,

                   const float &rotation,

                   const unsigned int &equipHand,
                   const unsigned int &equipOffhand,
                   const unsigned int &equipHead,
                   const unsigned int &equipBody,
                   const unsigned int &equipFeet,

                   const unsigned int &health,

                   const QBitArray &options,

                   const float &unknownFloat1,
                   const float &unknownFloat2,
                   const float &unknownFloat3,
                   const float &unknownFloat4,
                   QObject * parent = 0);

    // Builder. This returns a Human which is all set up.
    static Human * build(QStringList & unitData);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    QString filterString() const;

    // Setters for properties
    void setProfession(QString profession);

    // Getters for properties
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
    inline unsigned int tailorLevel() const { return m_tailorLevel; }
    inline unsigned int unknownUnit1Level() const { return m_unknownUnit1Level; }
    inline unsigned int unknownUnit2Level() const { return m_unknownUnit2Level; }

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

    inline bool option(unsigned int optionNumber) const { return m_options[optionNumber]; }

    inline float unknownFloat1() const { return m_unknownFloat1; }
    inline float unknownFloat2() const { return m_unknownFloat2; }
    inline float unknownFloat3() const { return m_unknownFloat3; }
    inline float unknownFloat4() const { return m_unknownFloat4; }

    // Utilities and Helpers
    void print();

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
    unsigned int m_tailorLevel;
    unsigned int m_unknownUnit1Level;
    unsigned int m_unknownUnit2Level;

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

    QBitArray m_options; // These contain the options for this unit.

    float m_unknownFloat1;
    float m_unknownFloat2;
    float m_unknownFloat3;
    float m_unknownFloat4;
};

// HumanListModel store Humans
class HumanListModel : public ListModel
{
private:
    Q_OBJECT

public:
    HumanListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long id, const QVariant &value, int role = Human::ProfessionRole);
    Q_INVOKABLE void remove(const long id);
    Q_INVOKABLE float getFirstPosition(const char label);
    Q_INVOKABLE void add(const QString type, float x, float y, float z);
};

#endif // HUMANLISTMODEL_H
