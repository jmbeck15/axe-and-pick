#include "humanlistmodel.h"

#include <QDebug>
#include <sstream>

Human::Human(const long &id,
             const QString &profession,
             const QString &name,
             QObject * parent)
    : ListItem(parent),
      m_id(id),
      m_profession(profession),
      m_name(name)
{
}

void Human::setProfession(QString profession)
{
    // Placeholder for when things are changed.
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
    case PosYRole:
    case PosZRole:
    case NameRole:
        return name();

    case ArcherLevelRole:
    case BlacksmithLevelRole:
    case BuilderLevelRole:
    case CarpenterLevelRole:
    case EngineerLeveRole:
    case FarmerLevelRole:
    case FishermanLevelRole:
    case ForagerLevelRole:
    case InfantryLevelRole:
    case MinerLevelRole:
    case StoneMasonLevelRole:
    case WoodChopperLevelRole:

    case ExperienceRole:

    case AutoChopTreesRole:
    case GatherBerriesRole:
    case HuntChickenRole:
    case HuntBoarRole:
    case ShowBowRangeRole:
    case TrainNearTargetRole:

    case RotationRole:

    case EquipHandRole:
    case EquipOffhandRole:
    case EquipHeadRole:
    case EquipBodyRole:
    case EquipFeetRole:

    case HealthRole:

    case AutoequipRole:
    case ReturnToCampfireRole:

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
