#ifndef HUMANLISTMODEL_H
#define HUMANLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QBitArray>
#include <QFile>
#include <QTextStream>

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
        TraderLevelRole,
        HerderLevelRole,
        AdventurerLevelRole,
        Unknown1Role,
        Unknown2Role,
        Unknown3Role,
        Unknown4Role,

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

        MoraleRole,
        FatigueRole,
        HungerRole

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
                   const unsigned int &traderLevel,
                   const unsigned int &herderLevel,
                   const unsigned int &adventurerLevel,
                   const unsigned int &unknown1Level,
                   const unsigned int &unknown2Level,
                   const unsigned int &unknown3Level,
                   const unsigned int &unknown4Level,

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
                   const float &morale,
                   const float &fatigue,
                   const float &hunger,

                   // Inventory data
                   const QList<int> &inventoryPreferences,
                   const QList<int> &inventoryItems,
                   const QList<int> &spareInventory,

                   // Patrol data
                   const QList<float> &patrolSetpoints,
                   const int &patrolIndex,

                   const QString &gardedUnitName,

                   const QList<int> &professionEXP,

                   const float &maxWeight,
                   QObject * parent = 0);

    // Builder. This returns a Human which is all set up.
    static Human * build(QStringList & unitData);

    // Writer. Writes the entire Human class to a file in the Timber and Stone format.
    void Human::writeToFile( QFile &unitFile );

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    QString filterString() const;

    // Setters for properties
    void setProfession(QString profession);
    void setMorale(float morale);
    void setFatigue(float fatigue);
    void setHunger(float hunger);

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
    inline unsigned int traderLevel() const { return m_traderLevel; }
    inline unsigned int herderLevel() const { return m_herderLevel; }
    inline unsigned int adventurerLevel() const { return m_adventurerLevel; }
    inline unsigned int unknown1Level() const { return m_unknown1Level; }
    inline unsigned int unknown2Level() const { return m_unknown2Level; }
    inline unsigned int unknown3Level() const { return m_unknown3Level; }
    inline unsigned int unknown4Level() const { return m_unknown4Level; }


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

    inline float timeToEat() const { return m_timeToEat; }
    inline float morale() const { return m_morale; }
    inline float fatigue() const { return m_fatigue; }
    inline float hunger() const { return m_hunger; }

    inline QList<int> const * inventoryPreferences() const { return &m_inventoryPreferences; }
    inline QList<int> const * inventoryItems() const { return &m_inventoryItems; }
    inline QList<int> const * spareInventory() const { return &m_spareInventory; }

    inline QList<float> const * patrolSetpoints() const { return &m_patrolSetpoints; }
    inline int patrolIndex() const { return m_patrolIndex; }

    inline QString guardedUnit() const { return m_guardedUnit; }

    inline QList<int> const * professionEXP() const { return &m_professionEXP; }

    inline float maxWeight() { return m_maxWeight; }

    // Utilities and Helpers
    void print();

private:
    long m_id;

    QString m_profession;
    float m_posX;
    float m_posY;
    float m_posZ;
    QString m_name;

    unsigned int m_archerLevel;     // also Merchant: times traded
    unsigned int m_blacksmithLevel; // also Merchant: total trades
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
    unsigned int m_traderLevel;
    unsigned int m_herderLevel;
    unsigned int m_adventurerLevel;
    unsigned int m_unknown1Level;
    unsigned int m_unknown2Level;
    unsigned int m_unknown3Level;
    unsigned int m_unknown4Level;

    unsigned int m_experience;

    bool m_autoChop;
    bool m_gatherBerries;
    bool m_huntChicken;
    bool m_huntBoar;
    bool m_showBowRange;

    // This is also used for "already met", for the Merchant
    bool m_trainNearTarget;

    float m_rotation;

    unsigned int m_equipHand;
    unsigned int m_equipOffhand;
    unsigned int m_equipHead;
    unsigned int m_equipBody;
    unsigned int m_equipFeet;

    unsigned int m_health;

    QBitArray m_options; // These contain the options for this unit.

    float m_timeToEat;
    float m_morale;
    float m_fatigue;
    float m_hunger;

    // Inventory data
    QList<int> m_inventoryPreferences;
    QList<int> m_inventoryItems;
    QList<int> m_spareInventory;

    // Patrol data
    QList<float> m_patrolSetpoints;
    int m_patrolIndex;

    QString m_guardedUnit;

    QList<int> m_professionEXP;

    float m_maxWeight;
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
    Q_INVOKABLE void add(const QString profession, float x, float y, float z);
    Q_INVOKABLE void serveCoffee();
};

#endif // HUMANLISTMODEL_H
