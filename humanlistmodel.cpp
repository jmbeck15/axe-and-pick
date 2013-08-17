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

             const float &timeToEat,
             const float &moral,
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
    m_unknown1Level(unknown1Level),
    m_unknown2Level(unknown2Level),
    m_unknown3Level(unknown3Level),
    m_unknown4Level(unknown4Level),

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

    // Inventory data
    m_inventoryPreferences(inventoryPreferences),
    m_inventoryItems(inventoryItems),
    m_spareInventory(spareInventory),

    // Patrol data
    m_patrolSetpoints(patrolSetpoints),
    m_patrolIndex(patrolIndex),

    m_guardedUnit(gardedUnitName),

    m_professionEXP(professionEXP),

    m_maxWeight(maxWeight)

{
    id_counter++;
}

Human * Human::build(QStringList & unitData)
{

    // Run a sanity check to make sure the version of
    // this save file is what we expect.
    //
    if (false) {
        return Q_NULLPTR;
    }

    QString unitProfession = unitData[0];

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
    // NOTE: There are 20 professions and associated levels.
    const int NumberOfProfessions = 20;
    for (int job=0; job<NumberOfProfessions; job++)
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
    for(unsigned int i=0; i<52; i++) // array[20] through array[71]
    {
        loadedOptions[i] = unitData[i+20].compare("True") ? false : true;
    }
    for(unsigned int i=52; i<52+12; i++) // array[76] through array[87]
    {
        loadedOptions[i] = unitData[i-52+76].compare("True") ? false : true;
    }

    // These items are not used for the Merchant, which is for some stupid
    // reason, saved with the other units.
    QList<int> inventoryPreferences;
    int numItemsInInventory;
    QList<int> inventoryItems;
    int numSpareInventory;
    QList<int> spareInventory;
    int numPatrolPoints;
    QList<float> patrolSetpoints;
    int patrolIndex;
    QString gardedUnitName;
    QList<int> professionEXP;
    int maxWeight;

    if( unitProfession != "Merchant" )
    {
        // Inventory Preferences
        for(int i=0; i<NumberOfProfessions; i++)
        {
            inventoryPreferences.append(unitData[88 + i].toInt());
        }

        // Inventory Items
        numItemsInInventory = unitData[108].toInt(); // num2
        for(int i=0; i<numItemsInInventory; i++)
        {
            inventoryItems.append(unitData[109 + i].toInt());
        }

        // Spare Inventory (slots?)
        numSpareInventory = unitData[109 + numItemsInInventory].toInt(); // num3
        for(int i=0; i<numSpareInventory; i++)
        {
            spareInventory.append(unitData[110 + numItemsInInventory + i].toInt());
        }

        // Patrol
        numPatrolPoints = unitData[110 + numItemsInInventory + numSpareInventory].toInt(); // num5
        for(int i=0; i<numPatrolPoints*3; i++ )
        {
            patrolSetpoints.push_back(unitData[111 + numItemsInInventory + numSpareInventory + i].toFloat());
        }

        patrolIndex = unitData[111 + numItemsInInventory + numSpareInventory + numPatrolPoints*3].toInt();

        // Guarded Unit
        gardedUnitName = unitData[112 + numItemsInInventory + numSpareInventory + numPatrolPoints*3];

        // Profession Experience
        for(unsigned int i=0; i<NumberOfProfessions; i++)
        {
            professionEXP.append(unitData[113 + numItemsInInventory + numSpareInventory + numPatrolPoints*3 + i].toInt());
        }

        maxWeight = unitData[133 + numItemsInInventory + numSpareInventory + numPatrolPoints*3].toFloat();
    }

    return (new Human(
                unitProfession,
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
                levels[16],
                levels[17],
                levels[18],
                levels[19],

                Utils::toInt(unitData[6].toLocal8Bit()), // experience

                unitData[7].compare("True") ? false : true,
                unitData[8].compare("True") ? false : true,
                unitData[9].compare("True") ? false : true,
                unitData[10].compare("True") ? false : true,
                unitData[11].compare("True") ? false : true,
                unitData[12].compare("True") ? false : true,

                unitData[13].toFloat(), // rotation

                Utils::toInt(unitData[14].toLocal8Bit()),   // weapon right
                Utils::toInt(unitData[15].toLocal8Bit()),   // weapon left
                Utils::toInt(unitData[16].toLocal8Bit()),   // helm
                Utils::toInt(unitData[17].toLocal8Bit()),   // chest
                Utils::toInt(unitData[18].toLocal8Bit()),   // boots

                Utils::toInt(unitData[19].toLocal8Bit()),   // health

                loadedOptions,

                unitData[72].toInt(), // time to eat
                unitData[73].toFloat(), // morale level
                unitData[74].toFloat(), // fatigue level
                unitData[75].toFloat(), // hunger level

                // Inventory data
                inventoryPreferences,
                inventoryItems,
                spareInventory,

                // Patrol data
                patrolSetpoints,
                patrolIndex,

                gardedUnitName,

                professionEXP,

                maxWeight
               ));

}

/**
 * @brief Write the Human data back to a file.
 */
void Human::writeToFile( QFile &unitFile )
{
    QTextStream unitStream(&unitFile);

    unitStream << profession() << "/"
               << QString("%1").arg(posX(),0,'g',8) << "/"
               << QString("%1").arg(posY(),0,'g',8) << "/"
               << QString("%1").arg(posZ(),0,'g',8) << "/"
               << name() << "/";
    unitStream.flush();

    unitFile.write(Utils::toBinary(archerLevel()));
    unitFile.write(Utils::toBinary(blacksmithLevel()));
    unitFile.write(Utils::toBinary(builderLevel()));
    unitFile.write(Utils::toBinary(carpenterLevel()).constData());
    unitFile.write(Utils::toBinary(engineerLevel()).constData());
    unitFile.write(Utils::toBinary(farmerLevel()).constData());
    unitFile.write(Utils::toBinary(fishermanLevel()).constData());
    unitFile.write(Utils::toBinary(foragerLevel()).constData());
    unitFile.write(Utils::toBinary(infantryLevel()).constData());
    unitFile.write(Utils::toBinary(minerLevel()).constData());
    unitFile.write(Utils::toBinary(stoneMasonLevel()).constData());
    unitFile.write(Utils::toBinary(woodChopperLevel()).constData());
    unitFile.write(Utils::toBinary(tailorLevel()).constData());
    unitFile.write(Utils::toBinary(traderLevel()).constData());
    unitFile.write(Utils::toBinary(herderLevel()).constData());
    unitFile.write(Utils::toBinary(adventurerLevel()).constData());
    unitFile.write(Utils::toBinary(unknown1Level()).constData());
    unitFile.write(Utils::toBinary(unknown2Level()).constData());
    unitFile.write(Utils::toBinary(unknown3Level()).constData());
    unitFile.write(Utils::toBinary(unknown4Level()).constData());
    unitStream << "/"; unitStream.flush();

    unitFile.write(Utils::toBinary(experience()).constData());
    unitStream << "/"; unitStream.flush();

    unitStream << QString(autoChop()?"True":"False") << "/"
               << QString(gatherBerries()?"True":"False") << "/"
               << QString(huntChicken()?"True":"False") << "/"
               << QString(huntBoar()?"True":"False") << "/"
               << QString(showBowRange()?"True":"False") << "/";

    unitStream << QString(trainNearTarget()?"True":"False") << "/";

    unitStream << QString("%1").arg(rotation(),0,'g',8) << "/";
    unitStream.flush();

    unitFile.write(Utils::toBinary(equipHand()).constData());
    unitStream << "/"; unitStream.flush();
    unitFile.write(Utils::toBinary(equipOffhand()).constData());
    unitStream << "/"; unitStream.flush();
    unitFile.write(Utils::toBinary(equipHead()).constData());
    unitStream << "/"; unitStream.flush();
    unitFile.write(Utils::toBinary(equipBody()).constData());
    unitStream << "/"; unitStream.flush();
    unitFile.write(Utils::toBinary(equipFeet()).constData());
    unitStream << "/"; unitStream.flush();
    unitFile.write(Utils::toBinary(health()).constData());
    unitStream << "/"; unitStream.flush();

    // Dump some of the options in the file.
    for (unsigned int i = 0; i<52; i++) {
        unitStream << QString(option(i)?"True":"False") << "/";
    }
    unitStream << timeToEat() << "/"
               << morale() << "/"
               << fatigue() << "/"
               << hunger() << "/";

    // Dump more options in the file.
    for (unsigned int i = 52; i<52+12; i++) {
        unitStream << QString(option(i)?"True":"False") << "/";
    }

    // Inventory Preferences
    for (int i = 0; i<inventoryPreferences()->length(); i++) {
        unitStream << inventoryPreferences()->at(i) << "/";
    }

    // Inventory Items
    unitStream << inventoryItems()->length() << "/";
    for (int i = 0; i<inventoryItems()->length(); i++) {
        unitStream << inventoryItems()->at(i) << "/";
    }

    // Spare Inventory
    unitStream << spareInventory()->length() << "/";
    for (int i = 0; i<spareInventory()->length(); i++) {
        unitStream << spareInventory()->at(i) << "/";
    }

    // Patrol
    unitStream << patrolSetpoints()->length() << "/";
    for (int i = 0; i<patrolSetpoints()->length(); i++) {
        unitStream << patrolSetpoints()->at(i) << "/";
    }
    unitStream << patrolIndex() << "/";

    unitStream << guardedUnit() << "/";

    // Profession Experience
    for (int i = 0; i<professionEXP()->length(); i++) {
        unitStream << professionEXP()->at(i) << "/";
    }

    unitStream << maxWeight() << "/";

    unitStream << endl;
    unitStream.flush();

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
    names[Unknown1Role] = "unknown1";
    names[Unknown2Role] = "unknown2";
    names[Unknown3Role] = "unknown3";
    names[Unknown4Role] = "unknown4";

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
    case Unknown1Role:
        return unknown1Level();
    case Unknown2Role:
        return unknown2Level();
    case Unknown3Role:
        return unknown3Level();
    case Unknown4Role:
        return unknown4Level();

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

    QList<int> inventoryPreferences;
    for(int i=0; i<20; i++) {
        inventoryPreferences.append(0);
    }
    QList<int> inventoryItems;
    QList<int> spareInventory;
    QList<float> patrolSetpoints;

    QList<int> professionEXP;
    for(int i=0; i<20; i++) {
        professionEXP.append(0);
    }

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
                  (profession.compare("Unknown1"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Unknown2"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Unknown3"))?qrand() % ((high + 1) - low) + low : expertLevel,
                  (profession.compare("Unknown4"))?qrand() % ((high + 1) - low) + low : expertLevel,

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

                  10.0f, 1.0f, 1.50f, 0.0f,   // time to eat, marole, fatigue, hunger

                  // Inventory data
                  inventoryPreferences,
                  inventoryItems,
                  spareInventory,

                  // Patrol data
                  patrolSetpoints,
                  0,

                  "NoUnit",  // person they're guarding.

                  professionEXP,

                  // There may be a desire to increase the max weight, perhaps when
                  // giving a unit coffee.
                  10 // max weight (it seems 10 is the default)
                  )
              );

    qDebug() << "Added" << randomlyChosenName << "the" << qPrintable((isFemale)?"female":"male") << qPrintable(profession);
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

