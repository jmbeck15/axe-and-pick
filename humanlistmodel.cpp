#include "humanlistmodel.h"

#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QCoreApplication>
#include <sstream>


#include "utils.h"


// Initialize the counter
long Human::id_counter = 0;

Human::Human(const QString &profession,
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

             const float &timeToEat,
             const float &moral,
             const float &fatigue,
             const float &hunger,

             const int &patrolCount,
             const QList<float> &patrolSetpoints,
             const int &patrolIndex,

             const QString &guardedUnit,
             QObject * parent)
    : ListItem(parent),
    m_id(id_counter),

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
    m_tailorLevel(tailorLevel),
    m_traderLevel(traderLevel),
    m_herderLevel(herderLevel),
    m_adventurerLevel(adventurerLevel),

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

    m_options(options),

    m_timeToEat(timeToEat),
    m_morale(moral),
    m_fatigue(fatigue),
    m_hunger(hunger),

    m_patrolCount(patrolCount),
    m_patrolSetpoints(patrolSetpoints),
    m_patrolIndex(patrolIndex),

    m_guardedUnit(guardedUnit)
{
    id_counter++;
}

Human * Human::build(QStringList & unitData)
{

    // Check to make sure there are the proper number of options.
    //
    // NOTE: This is only a sanity check. This can fail.
    // The algo says "it has to be at least 92 long, and anything
    // above 92 has to be divisible by three, to account for the
    // patrol points".
    //
    if (    (unitData.size() >= 92)
         && ((unitData.size() - 92) % 3 == 0)
       ) {

        //
        // Experience Levels
        //
        QByteArray rawLevelString( unitData[5].toLocal8Bit() );
        QByteArray numberInBytes;
        numberInBytes.clear();

        unsigned char byte(0);
        unsigned int byteNumber(0);
        QList<unsigned int> levels;
        levels.clear();

        // For each job, compute the experience level.
        // NOTE: There are 15 professions and associated levels.
        for (int job=0; job<16; job++)
        {
            // Most significant byte
            byte = rawLevelString[byteNumber];
            byteNumber++;

            if( byte >= 0xe0)
            {
                numberInBytes.append(byte);

                // Another byte exists!
                byte = rawLevelString[byteNumber];
                byteNumber++;
                numberInBytes.append(byte);
            }
            else
            {
                numberInBytes.append(byte);
            }

            byte = rawLevelString[byteNumber];
            byteNumber++;
            numberInBytes.append(byte);

            // Convert the bytes to a long, and add to the list.
            levels.append(Utils::toInt(numberInBytes));

            numberInBytes.clear();
        }
        byteNumber = 0;


        //
        // Load the Options
        //
        QBitArray loadedOptions(64, false);
        for(unsigned int i=0; i<52; i++)
        {
            loadedOptions[i] = unitData[i+20].compare("True") ? false : true;
        }
        for(unsigned int i=52; i<52+12; i++)
        {
            loadedOptions[i] = unitData[i-52+76].compare("True") ? false : true;
        }

        //
        // Patrol Values
        // Starts at 88
        int patrolCount = unitData[88].toInt();

        // Add all the patrol waypoints to the list
        // NOTE: I realize this is xyz. Better to have a class
        // that wraps each point. But I'm not even using these right
        // now, so I'll decide that when/if I ever need it.
        QList<float> patrolSetpoints;
        for(int patrolCountNdx=0; patrolCountNdx<patrolCount*3; patrolCountNdx++ )
        {
            patrolSetpoints.push_back(unitData[89+patrolCountNdx].toFloat());
        }
        int patrolIndex = unitData[89+patrolCount*3].toInt();

        return (new Human(
                    unitData[0],
                    unitData[1].toFloat(),
                    unitData[2].toFloat(),
                    unitData[3].toFloat(),
                    unitData[4],

                    levels[0],
                    levels[1],
                    levels[2],
                    levels[3],
                    levels[4],
                    levels[5],
                    levels[6],
                    levels[7],
                    levels[8],
                    levels[9],
                    levels[10],
                    levels[11],
                    levels[12],
                    levels[13],
                    levels[14],
                    levels[15],

                    Utils::toInt(unitData[6].toLocal8Bit()), // experience

                    unitData[7].compare("True") ? false : true,
                    unitData[8].compare("True") ? false : true,
                    unitData[9].compare("True") ? false : true,
                    unitData[10].compare("True") ? false : true,
                    unitData[11].compare("True") ? false : true,
                    unitData[12].compare("True") ? false : true,

                    unitData[13].toFloat(), // rotation

                    Utils::toInt(unitData[14].toLocal8Bit()),
                    Utils::toInt(unitData[15].toLocal8Bit()),
                    Utils::toInt(unitData[16].toLocal8Bit()),
                    Utils::toInt(unitData[17].toLocal8Bit()),
                    Utils::toInt(unitData[18].toLocal8Bit()),

                    Utils::toInt(unitData[19].toLocal8Bit()),

                    loadedOptions,

                    unitData[72].toFloat(), // unknown options
                    unitData[73].toFloat(), // morale
                    unitData[74].toFloat(), // fatigue
                    unitData[75].toFloat(), // hunger

                    patrolCount,            // Patrol data
                    patrolSetpoints,
                    patrolIndex,

                    unitData[89+patrolCount*3+1]            // guarded unit
                   ));
    }

    return Q_NULLPTR;
}

void Human::setProfession(QString profession)
{
    // Placeholder for when things are changed.
    m_profession = profession;
}

void Human::setMorale(float morale)
{
    // Placeholder for when things are changed.
    m_morale = morale;
}
void Human::setFatigue(float fatigue)
{
    // Placeholder for when things are changed.
    m_fatigue = fatigue;
}
void Human::setHunger(float hunger)
{
    // Placeholder for when things are changed.
    m_hunger = hunger;
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
    names[TailorLevelRole] = "tailorLevel";
    names[TraderLevelRole] = "traderLevel";
    names[HerderLevelRole] = "herderLevel";
    names[AdventurerLevelRole] = "adventurerLevel";

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

    names[MoraleRole] = "marole";
    names[FatigueRole] = "fatigue";
    names[HungerRole] = "hunger";

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
    case TailorLevelRole:
        return tailorLevel();
    case TraderLevelRole:
        return traderLevel();
    case HerderLevelRole:
        return herderLevel();
    case AdventurerLevelRole:
        return adventurerLevel();

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

    case MoraleRole:
        return morale();
    case FatigueRole:
        return fatigue();
    case HungerRole:
        return hunger();

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
    case Human::MoraleRole:
    {
        Human * item = (Human *)find( id );
        item->setMorale(value.toFloat());
        break;
    }
    case Human::FatigueRole:
    {
        Human * item = (Human *)find( id );
        item->setFatigue(value.toFloat());
        break;
    }
    case Human::HungerRole:
    {
        Human * item = (Human *)find( id );
        item->setHunger(value.toFloat());
        break;
    }
    default:
        qWarning() << "HumanListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

void HumanListModel::remove(const long id)
{
    // Find the id of the item you want to delete,
    // get the index of that item, and remove it.
    removeRow(indexFromItem(find( id )).row());
}

// This function only exists because we can't parse the map file.
// TODO: change how units are placed.
float HumanListModel::getFirstPosition(const char label)
{
    if (getList().size() != 0) {
        if (label==0) {
            return ((Human*)getList().first())->posX();
        }
        else if (label==1) {
            return ((Human*)getList().first())->posY();
        }
        else if (label==2) {
            return ((Human*)getList().first())->posZ();
        }
    }
    return 0.0;
}

void HumanListModel::add(const QString profession, float x, float y, float z)
{
    int high = 5;
    int low = 1;

    // Set the options
    QBitArray options(52+12, false);
    for (int i=2; i<13; i++) {
        options[i] = true;  // These settings seem to be on by default.
    }
    options[15] = true; // Set default sleep settings.
    options[18] = true;

    // Randomly choose male or female (40 percent chance of female: the in-game
    // chance is about 33 percent, but that seems a little low somehow.)
    high = 100;
    low = 0;
    bool isFemale = ((qrand() % ((high + 1) - low) + low) > 60) ? true : false;

    // Choose the proper names file for male/female units, and set the default
    // name in case we can't get a random name from the names file.
    QString namesFilename;
    QString randomlyChosenName;
    if (isFemale) {
        namesFilename = "/female_names.txt";
        randomlyChosenName = "Alissa";
    }
    else {
        namesFilename = "/male_names.txt";
        randomlyChosenName = "James";
    }
    // Set Gender
    options[37] = isFemale;

    // Pick a name from the list of names. If you can't find the file, the default is okay.
    QFile namesFile(QCoreApplication::applicationDirPath() + namesFilename);
    if (namesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream namesStream(&namesFile);
        QString namesString = namesStream.readAll();
        QStringList names = namesString.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);

        // Pick a random name
        randomlyChosenName = names.at(qrand() % names.size());
    }

    QList<float> patrolSetpoints;
    patrolSetpoints.clear();

    // Build the Human and add it to the list.

    // Randomly choose the expert level, used for the chosen profession.
    int expertLevel = qrand() % ((8 + 1) - 6) + 6;
    high = 5;
    low = 1;
    appendRow(new Human(
                  profession,
                  x, y, z,
                  randomlyChosenName + " Drone",

                  // The level is raised to expert level if that's this units profession.
                  (profession.compare("Archer"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Blacksmith"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Builder"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Carpenter"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Engineer"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Farmer"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Fisherman"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Forager"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Infantry"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Miner"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Stone Mason"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Wood Chopper"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Tailor"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Trader"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Herder"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Adventurer"))?qrand() % ((high + 1) - low) + low : expertLevel,

                  1, // experience

                  false,
                  false,
                  false,
                  false,
                  false,
                  false,

                  0.0, // rotation

                  127,127,127,127,127, // equip

                  100, // health

                  options,

                  10.0f, 1.0f, 1.52f, 0.0f,   // unknown, marole, fatigue, hunger

                  0, patrolSetpoints, 0,  // patrol count, patrol setpoints, patrol index

                  "NoUnit"  // person they're guarding.
                  )
              );

    qDebug() << "Added" << randomlyChosenName << "the" << qPrintable((isFemale)?"male":"female") << qPrintable(profession);
}

void HumanListModel::serveCoffee()
{
    QList<ListItem*> list;
    list = getList();
    foreach(ListItem * human, list)
    {
        ((Human*)human)->setMorale(1.0f);
        ((Human*)human)->setFatigue(1.52f);  // 1.5 is the limit, so this is an extra boost!
        ((Human*)human)->setHunger(0.0f);
    }
}

