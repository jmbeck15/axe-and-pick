#include "humanlistmodel.h"

#include <QDebug>
#include <sstream>

Human::Human(const long &id,
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
             QObject * parent)
    : ListItem(parent),
    m_id(id),

    m_profession(profession),
    m_posX(posX),
    m_posY(posY),
    m_posZ(posZ),
    m_name(name),

    m_archerLevel(archerLevel),
    m_blacksmithLevel(blacksmithLevel),
    m_builderLevel(builderLevel),
    m_carpenterLevel(carpenterLevel),
    m_engineerLevel(engineerLevel),
    m_farmerLevel(farmerLevel),
    m_fishermanLevel(fishermanLevel),
    m_foragerLevel(foragerLevel),
    m_infantryLevel(infantryLevel),
    m_minerLevel(minerLevel),
    m_stoneMasonLevel(stoneMasonLevel),
    m_woodChopperLevel(woodChopperLevel),

    m_experience(experience),

    m_autoChop(autoChop),
    m_gatherBerries(gatherBerries),
    m_huntChicken(huntChicken),
    m_huntBoar(huntBoar),
    m_showBowRange(showBowRange),
    m_trainNearTarget(trainNearTarget),

    m_rotation(rotation),

    m_equipHand(equipHand),
    m_equipOffhand(equipOffhand),
    m_equipHead(equipHead),
    m_equipBody(equipBody),
    m_equipFeet(equipFeet),

    m_health(health),

    m_autoEquip(autoEquip),
    m_returnToCampfire(returnToCampfire)
{
}

void Human::setProfession(QString profession)
{
    // Placeholder for when things are changed.
    m_profession = profession;
}

QHash<int, QByteArray> Human::roleNames() const
{
    QHash<int, QByteArray> names;

    // These names are the functions which return the values.
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    names[ProfessionRole] = "profession";
    names[PosXRole] = "posX";
    names[PosYRole] = "posY";
    names[PosZRole] = "posZ";
    names[NameRole] = "name";

    names[ArcherLevelRole] = "archerLevel";
    names[BlacksmithLevelRole] = "blacksmithLevel";
    names[BuilderLevelRole] = "builderLevel";
    names[CarpenterLevelRole] = "carpenterLevel";
    names[EngineerLeveRole] = "engineerLevel";
    names[FarmerLevelRole] = "farmerLevel";
    names[FishermanLevelRole] = "fishermanLevel";
    names[ForagerLevelRole] = "foragerLevel";
    names[InfantryLevelRole] = "infantryLevel";
    names[MinerLevelRole] = "minerLevel";
    names[StoneMasonLevelRole] = "stoneMasonLevel";
    names[WoodChopperLevelRole] = "woodChopperLevel";

    names[ExperienceRole] = "experience";

    names[AutoChopTreesRole] = "autoChop";
    names[GatherBerriesRole] = "gatherBerries";
    names[HuntChickenRole] = "huntChicken";
    names[HuntBoarRole] = "huntBoar";
    names[ShowBowRangeRole] = "showBowRange";
    names[TrainNearTargetRole] = "trainNearTarget";

    names[RotationRole] = "rotation";

    names[EquipHandRole] = "equipHand";
    names[EquipOffhandRole] = "equipOffhand";
    names[EquipHeadRole] = "equipHead";
    names[EquipBodyRole] = "equipBody";
    names[EquipFeetRole] = "equipFeet";

    names[HealthRole] = "health";

    names[AutoequipRole] = "autoEquip";
    names[ReturnToCampfireRole] = "returnToCampfire";

    return names;
}

QVariant Human::data(int role) const
{
    switch(role) {
    case IdRole:
        return (unsigned int)id();
    case FilterStringRole:
        return filterString();

    case ProfessionRole:
        return profession();
    case PosXRole:
        return posX();
    case PosYRole:
        return posY();
    case PosZRole:
        return posZ();
    case NameRole:
        return name();

    case ArcherLevelRole:
        return archerLevel();
    case BlacksmithLevelRole:
        return blacksmithLevel();
    case BuilderLevelRole:
        return builderLevel();
    case CarpenterLevelRole:
        return carpenterLevel();
    case EngineerLeveRole:
        return engineerLevel();
    case FarmerLevelRole:
        return farmerLevel();
    case FishermanLevelRole:
        return fishermanLevel();
    case ForagerLevelRole:
        return foragerLevel();
    case InfantryLevelRole:
        return infantryLevel();
    case MinerLevelRole:
        return minerLevel();
    case StoneMasonLevelRole:
        return stoneMasonLevel();
    case WoodChopperLevelRole:
        return woodChopperLevel();

    case ExperienceRole:
        return experience();

    case AutoChopTreesRole:
        return autoChop();
    case GatherBerriesRole:
        return gatherBerries();
    case HuntChickenRole:
        return huntChicken();
    case HuntBoarRole:
        return huntBoar();
    case ShowBowRangeRole:
        return showBowRange();
    case TrainNearTargetRole:
        return trainNearTarget();

    case RotationRole:
        return rotation();

    case EquipHandRole:
        return equipHand();
    case EquipOffhandRole:
        return equipOffhand();
    case EquipHeadRole:
        return equipHead();
    case EquipBodyRole:
        return equipBody();
    case EquipFeetRole:
        return equipFeet();

    case HealthRole:
        return health();

    case AutoequipRole:
        return autoEquip();
    case ReturnToCampfireRole:
        return returnToCampfire();

    default:
        return QVariant();
    }
}

QString Human::filterString() const
{
    std::stringstream completeString;

    completeString <<        name().toStdString()
                   << " " << profession().toStdString();
    return (QString(completeString.str().c_str()));
}

void Human::print()
{
    qDebug() << name() << "-" << profession();
    qDebug() << "   Exp:" << experience() << "  Health:" << health();
    qDebug() << "   Auto-equip:" << autoEquip() << "   Auto-chop:" << autoChop();
    qDebug() << "   Return to campfire:" << returnToCampfire();
}


HumanListModel::HumanListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void HumanListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case Human::ProfessionRole:
    {
        Human * item = (Human *)find( id );
        item->setProfession(value.toString());
        break;
    }
    default:
        qWarning() << "HumanListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

