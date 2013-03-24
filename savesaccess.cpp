#include "savesaccess.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent),
    savedGameModel(Q_NULLPTR),
    resourceModel(Q_NULLPTR)
{
}

// Sets the path and loads the game.
void SavesAccess::loadSavedGame(QString gameName)
{
    // Set the game name.
    selectedSaveName = gameName;

    // Pull the resources into the list.
    loadResourceFile();
    loadUnitFile();
}

void SavesAccess::saveSavedGame()
{
    saveResourceFile();
    saveUnitFile();
    //saveTimeToFile();
    //saveGameOverviewToFile();
}

void SavesAccess::openFileDialog()
{

    // TODO: Set the parent of this dialog so it returns properly.
    // Not sure if this is the cause of the weird Windows behavior
    // or not. Something to try.
    QFileDialog fileDialog(0,"Timber and Stone saves.sav file");
    fileDialog.setDirectory(rootSavesDirectory);
    fileDialog.setNameFilter("Saves File (saves.sav)");

    // Open the file dialog so the user can select the saves.sav file.
    if (fileDialog.exec())
    {
        rootSavesDirectory.setPath(fileDialog.selectedFiles().first());
        rootSavesDirectory.cdUp(); // This trims off the saves.sav file.
    }
}


// NOTE: Qml access this for types that are explicitly typed in.
void SavesAccess::setFilePath(QString path)
{
    // Save the path to the file.
    rootSavesDirectory.setPath(path);
    rootSavesDirectory.cdUp();
}

QString SavesAccess::getSavesPath()
{
    return (rootSavesDirectory.absolutePath() + "/" + "saves.sav");
}

bool SavesAccess::pathIsValid()
{
    // Return True if both the directory path is valid
    // and the saves.sav file exists.
    QFile savedGameFile(getSavesPath());
    QFileInfo fileInfo(savedGameFile);
    if (savedGameFile.exists() && (fileInfo.fileName() == "saves.sav"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SavesAccess::loadGamesList()
{
    // Save the file name.
    QFile savedGameFile(rootSavesDirectory.absolutePath()
                        + "/" + "saves.sav");

    if (savedGameModel == Q_NULLPTR)
    {
        qDebug() << "SavedAccess model hasn't been set up yet.";
        return;
    }

    // Open file and make sure it went okay.
    if (!savedGameFile.exists() || !savedGameFile.open(QFile::ReadOnly | QFile::Text))
    {
        // TODO: Make this error apparent on the interface somehow.
        qDebug() << "Can't open savedGameFile.";
        return;
    }
    else
    {
        QTextStream in(&savedGameFile);
        QStringList strings;

        long numberOfGames = in.readLine().toLongLong();
        qDebug() << "Loaded" << numberOfGames << "games.";

        // Remove all the games.
        savedGameModel->clear();

        // Add the games from the saves.sav file.
        while (!in.atEnd())
        {
            strings = in.readLine().split("/", QString::KeepEmptyParts);

            // Add the games to the list.
            savedGameModel->appendRow(new SavedGame(strings[0],
                                 strings[3],
                                 strings[4],
                                 strings[1].toLongLong(),
                                 strings[2].toLongLong()));
        }
    }

    // It's important to always close the file after reading/writing, so this
    // app can remain running while Timber and Stone saves as it wishes.
    savedGameFile.close();
}

void SavesAccess::setSavedGameListModel(SavedGameListModel * model)
{
    savedGameModel = model;
}

QByteArray SavesAccess::toBinary(unsigned int value)
{
    QByteArray bytes;
    bytes.clear();

    // NOTE: I do not understand this. I understand what it's doing, but
    // I have no idea why. I suspect it's much more simple, and I've
    // missed something.
    if (value>1919) // this is the largest two-byte number
    {
        bytes.append( (((value>>6) + 2)>>6) | 0xe0 );
        bytes.append( (((value>>6) + 2) &0x3f) | 0x80 );
    }
    else
    {
        bytes.append( (((value>>6) &0x1f) + 2) | 0xc0 );
    }
    bytes.append((value&0x3f) | 0x80);

    return bytes;
}

// This format handling can certainly be simplified. There's little
// chance that the game designer made it complicated on purpose.
//
unsigned int SavesAccess::toInt(QByteArray bytes)
{
    // Ensure the bytes have only their relevant bits visible.
    unsigned int value;
    if( bytes.size() > 2 )
    {
        bytes[0] = bytes[0] & 0x1f; // Two or more bytes exist, so this one has an extra id bit.
        bytes[1] = bytes[1] & 0x3f;
        bytes[2] = bytes[2] & 0x3f;

        // Compile the bytes into a long.
        //
        // Note: The -2 in the equation is strange. I have no answer as to why
        // this format requires that.
        value = bytes[2] | (((bytes[1] | (bytes[0]<<6)) -2) << 6);
    }
    else
    {
        bytes[0] = bytes[0] & 0x3f;
        bytes[1] = bytes[1] & 0x3f;
        value = bytes[1] | (bytes[0] -2) << 6;
    }

    return value;
}

// =====================
// RESOURCES
// =====================

void SavesAccess::setResourceListModel(ResourceListModel * model)
{
    resourceModel = model;
}

void SavesAccess::loadResourceFile()
{
    QFile resourceFile(rootSavesDirectory.absolutePath()
                       + "/" + selectedSaveName
                       + "/" + "re.sav");

    if (resourceModel == Q_NULLPTR)
    {
        qDebug() << "Resource model hasn't been set up yet.";
        return;
    }

    // Open file and make sure it went okay.
    if (!resourceFile.exists() || !resourceFile.open(QFile::ReadWrite))
    {
        // TODO: Make this error apparent on the interface somehow.
        qDebug() << "Can't open resourceFile.";
        return;
    }
    else
    {

        // Pull in the list of resource assets (name, group, etc)
        QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.csv");

        if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Opened Asset File: " << assetFile.fileName();

            // Clear the resource model
            resourceModel->clear();

            QTextStream assetStream(&assetFile);

            while (!assetStream.atEnd())
            {
                // Grab the next section and decide the size, etc.
                QString assetString;
                assetString = assetStream.readLine();
                QStringList assetData;
                assetData = assetString.split(',');

                // Temporary array to hold the bytes we read from the file.
                QByteArray byteArray;
                byteArray.clear();

                unsigned char byte; // Most Significant Byte
                resourceFile.read((char *)&byte, 1);

                if( byte >= 0xe0)
                {
                    byteArray.append(byte);

                    // Another byte exists!
                    resourceFile.read((char *)&byte, 1);
                    byteArray.append(byte);
                }
                else
                {
                    byteArray.append(byte);
                }

                resourceFile.read((char *)&byte, 1);
                byteArray.append(byte);

                resourceModel->appendRow(new Resource(assetData[0],
                                          assetData[1],
                                          assetData[2],
                                          toInt(byteArray))); // resource quantity
            }

            if (!resourceFile.atEnd())
            {
                // Read the rest of the file into an array, and keep it for later.
                resourceExtraData.clear();
                resourceExtraData = resourceFile.readAll();
            }
        }
        else
        {
            // TODO: Make this error apparent on the interface somehow.
            qDebug() << "Could not open " << assetFile.fileName();
        }

        assetFile.close();
        resourceFile.close();
    }

}


void SavesAccess::saveResourceFile()
{
    if (resourceModel == Q_NULLPTR
            || resourceModel->rowCount() <= 0)
    {
        qDebug() << "The resource model hasn't been populated.";
        return;
    }

    QFile resourceFile(rootSavesDirectory.absolutePath()
                       + "/" + selectedSaveName
                       + "/" + "re.sav");

    // Open file for write, and make sure it went okay.
    if (!resourceFile.open(QFile::ReadWrite))
    {
        // TODO: Make this error apparent on the interface somehow.
        // Perhaps by making the save icon be red instead of green.
        qDebug() << "Can't open resourceFile for writing.";
        return;
    }
    else
    {
        QByteArray binaryData;

        // Write the data to the resource file. Clobber the old one.
        for (int ndx = 0;
             ndx < resourceModel->rowCount();
             ndx++)
        {
            long quantity = resourceModel->index(ndx).data(Resource::QuantityRole).toLongLong();
            if (quantity > 30000)
            {
                qDebug() << "Resource quantities above 32,639 will fail to load in the game. Reduced to 30,000.";
                quantity = 30000;

                // Update the resource.
                resourceModel->setData(ndx,30000);
            }
            binaryData = toBinary(quantity);
            resourceFile.write(binaryData);
        }

        // Write out the unexpected data to the file too.
        resourceFile.write(resourceExtraData);
        resourceFile.close();
    }
}

// =====================
// UNITS (human, neutral mobs, violent mobs)
// =====================

void SavesAccess::setHumanModel(HumanListModel * model)
{
    humanModel = model;
}
void SavesAccess::setNeutralMobModel(NeutralMobListModel * model)
{
    neutralMobModel = model;
}
void SavesAccess::setViolentMobModel(ViolentMobListModel * model)
{
    violentMobModel = model;
}

void SavesAccess::loadUnitFile()
{
    // Compose the file name.
    QFile unitFile(rootSavesDirectory.absolutePath()
                   + "/" + selectedSaveName
                   + "/" + "un.sav");

    if ((humanModel == Q_NULLPTR) |
        (neutralMobModel == Q_NULLPTR) |
        (violentMobModel == Q_NULLPTR) )
    {
        qDebug() << "Human, neutral, violent mob models haven't been set up yet.";
        return;
    }

    // Open file and make sure it went okay.
    if (!unitFile.exists() || !unitFile.open(QFile::ReadWrite))
    {
        // TODO: Make this error apparent on the interface somehow.
        qDebug() << "Can't open unitFile.";
        return;
    }
    else
    {
        qDebug() << "Opened unitFile." << unitFile.fileName();

        // Remove all the humans.
        humanModel->clear();

        QTextStream unitStream(&unitFile);
        QString unitString;
        QStringList unitData;

        // Load in all the Humans
        int numberOfHumans = unitStream.readLine().toInt();
        qDebug() << "Loaded" << numberOfHumans << "humans";
        for (int i=0; i<numberOfHumans; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            // Break out the [5] element (experience levels) into numbers

            // Store the bytes in an array instead of a normal string.
            QByteArray rawLevelString( unitData[5].toLocal8Bit() );
            QByteArray numberInBytes;
            numberInBytes.clear();

            unsigned char byte(0);
            unsigned int byteNumber(0);
            QList<unsigned int> levels;
            levels.clear();


            // For each job, compute the experience level.
            // NOTE: There are 15 professions and associated levels.
            for (int job=0; job<15; job++)
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
                levels.append(toInt(numberInBytes));

                numberInBytes.clear();
            }
            byteNumber = 0;

            // Load in all the options at once.
            QBitArray loadedOptions(52);
            for(unsigned int i=0; i<52; i++)
            {
                loadedOptions[i] = unitData[i+20].compare("True") ? false : true;
            }

            humanModel->appendRow(new Human(
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

                                      toInt(unitData[6].toLocal8Bit()), // experience

                                      unitData[7].compare("True") ? false : true,
                                      unitData[8].compare("True") ? false : true,
                                      unitData[9].compare("True") ? false : true,
                                      unitData[10].compare("True") ? false : true,
                                      unitData[11].compare("True") ? false : true,
                                      unitData[12].compare("True") ? false : true,

                                      unitData[13].toFloat(), // rotation

                                      toInt(unitData[14].toLocal8Bit()),
                                      toInt(unitData[15].toLocal8Bit()),
                                      toInt(unitData[16].toLocal8Bit()),
                                      toInt(unitData[17].toLocal8Bit()),
                                      toInt(unitData[18].toLocal8Bit()),

                                      toInt(unitData[19].toLocal8Bit()),

                                      loadedOptions,

                                      unitData[72].toFloat(), // unknown options
                                      unitData[73].toFloat(),
                                      unitData[74].toFloat(),
                                      unitData[75].toFloat(),
                                      unitData[76].compare("True") ? false : true
                                    ));

            //((Human *)humanModel->find(i))->print();
        }

        // Load in all the Neutral Mobs
        int numberOfNeutralMobs = unitStream.readLine().toInt();
        qDebug() << "Loaded" << numberOfNeutralMobs << "neutral mobs";
        for (int i=0; i<numberOfNeutralMobs; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            neutralMobModel->appendRow(new NeutralMob(
                                       unitData[0],
                                       unitData[1].toFloat(),
                                       unitData[2].toFloat(),
                                       unitData[3].toFloat(),
                                       unitData[4].toFloat()) );
        }

        // Load in all the Violent Mobs
        int numberOfViolentMobs = unitStream.readLine().toInt();
        qDebug() << "Loaded" << numberOfViolentMobs << "violent mobs";
        for (int i=0; i<numberOfViolentMobs; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            violentMobModel->appendRow(new ViolentMob(
                                       unitData[0],
                                       unitData[1].toFloat(),
                                       unitData[2].toFloat(),
                                       unitData[3].toFloat(),
                                       unitData[4].toFloat(),
                                       unitData[5].toFloat()) );
        }

        unitFile.close();
    }

}

void SavesAccess::saveUnitFile()
{
    // Compile all the units into the unit file.

    if (   humanModel == Q_NULLPTR
        || neutralMobModel == Q_NULLPTR
        || violentMobModel == Q_NULLPTR )
    {
        qDebug() << "One of the unit models hasn't been set up yet.";
        return;
    }

    QFile unitFile(rootSavesDirectory.absolutePath()
                       + "/" + selectedSaveName
                       + "/" + "un.sav");


    // Open file for write, and make sure it went okay.
    if (!unitFile.open(QIODevice::WriteOnly))
    {
        // TODO: Make this error apparent on the interface somehow.
        // Perhaps by making the save icon be red instead of green.
        qDebug() << "Can't open unitFile for writing.";
        return;
    }
    else
    {
        QTextStream unitStream(&unitFile);

        // Save the humans, then the neutral mobs, then the violent mobs.

        // Humans
        unitStream << humanModel->rowCount() << endl;
        for (QList<ListItem*>::iterator itr = humanModel->getList().begin(); itr != humanModel->getList().end(); itr++)
        {
            Human* human = (Human*)*itr;

            unitStream << human->profession() << "/"
                       << QString("%1").arg(human->posX(),0,'g',8) << "/"
                       << QString("%1").arg(human->posY(),0,'g',8) << "/"
                       << QString("%1").arg(human->posZ(),0,'g',8) << "/"
                       << human->name() << "/";
            unitStream.flush();

            unitFile.write(toBinary(human->archerLevel()));
            unitFile.write(toBinary(human->blacksmithLevel()));
            unitFile.write(toBinary(human->builderLevel()));
            unitFile.write(toBinary(human->carpenterLevel()).constData());
            unitFile.write(toBinary(human->engineerLevel()).constData());
            unitFile.write(toBinary(human->farmerLevel()).constData());
            unitFile.write(toBinary(human->fishermanLevel()).constData());
            unitFile.write(toBinary(human->foragerLevel()).constData());
            unitFile.write(toBinary(human->infantryLevel()).constData());
            unitFile.write(toBinary(human->minerLevel()).constData());
            unitFile.write(toBinary(human->stoneMasonLevel()).constData());
            unitFile.write(toBinary(human->woodChopperLevel()).constData());
            unitFile.write(toBinary(human->tailorLevel()).constData());
            unitFile.write(toBinary(human->unknownUnit1Level()).constData());
            unitFile.write(toBinary(human->unknownUnit2Level()).constData());
            unitStream << "/"; unitStream.flush();

            unitFile.write(toBinary(human->experience()).constData());
            unitStream << "/"; unitStream.flush();

            unitStream << QString(human->autoChop()?"True":"False") << "/"
                       << QString(human->gatherBerries()?"True":"False") << "/"
                       << QString(human->huntChicken()?"True":"False") << "/"
                       << QString(human->huntBoar()?"True":"False") << "/"
                       << QString(human->showBowRange()?"True":"False") << "/"
                       << QString(human->trainNearTarget()?"True":"False") << "/"
                       << QString("%1").arg(human->rotation(),0,'g',8) << "/";
            unitStream.flush();

            unitFile.write(toBinary(human->equipHand()).constData());
            unitStream << "/"; unitStream.flush();
            unitFile.write(toBinary(human->equipOffhand()).constData());
            unitStream << "/"; unitStream.flush();
            unitFile.write(toBinary(human->equipHead()).constData());
            unitStream << "/"; unitStream.flush();
            unitFile.write(toBinary(human->equipBody()).constData());
            unitStream << "/"; unitStream.flush();
            unitFile.write(toBinary(human->equipFeet()).constData());
            unitStream << "/"; unitStream.flush();
            unitFile.write(toBinary(human->health()).constData());
            unitStream << "/"; unitStream.flush();

            // Dump all the options in the file.
            for (unsigned int i = 0; i<52; i++) {
                unitStream << QString(human->option(i)?"True":"False") << "/";
            }
            unitStream << human->unknownFloat1() << "/"
                       << human->unknownFloat2() << "/"
                       << human->unknownFloat3() << "/"
                       << human->unknownFloat4() << "/"
                       << QString(human->unknownBool1()?"True":"False") << "/";

            unitStream << endl;
            unitStream.flush();
        }

        // Neutral Mobs
        unitStream << neutralMobModel->rowCount() << endl;
        for (QList<ListItem*>::iterator itr = neutralMobModel->getList().begin(); itr != neutralMobModel->getList().end(); itr++)
        {
            NeutralMob* mob = (NeutralMob*)*itr;
            unitStream << mob->type() << "/"
                       << mob->posX() << "/"
                       << mob->posY() << "/"
                       << mob->posZ() << "/"
                       << mob->rotation() << "/";
            unitStream << endl;
        }

        // Violent Mobs
        unitStream << violentMobModel->rowCount() << endl;
        for (QList<ListItem*>::iterator itr = violentMobModel->getList().begin(); itr != violentMobModel->getList().end(); itr++)
        {
            ViolentMob* mob = (ViolentMob*)*itr;
            unitStream << mob->type() << "/"
                       << mob->posX() << "/"
                       << mob->posY() << "/"
                       << mob->posZ() << "/"
                       << mob->rotation() << "/"
                       << mob->health() << "/";
            unitStream << endl;
        }
    }

    unitFile.close();
}

void SavesAccess::writeToMatlab()
{
    qDebug() << "Opening file...";

    // Create a new file for storing all these points. Comma seperated.
    QFile matlabFile(rootSavesDirectory.absolutePath()
                    + "/" + selectedSaveName
                    + "/" + "cd.dat");
    matlabFile.open(QFile::WriteOnly);
    QTextStream matlabStream(&matlabFile);

    QFile worldFile(rootSavesDirectory.absolutePath()
                    + "/" + selectedSaveName
                    + "/" + "cd.sav");
    worldFile.open(QFile::ReadOnly);

    int xMax(0);
    int yMax(0);
    int zMax(0);

    while( !worldFile.atEnd() )
    {
        // Temporary array to hold the bytes we read from the file.
        QByteArray byteArray;
        byteArray.clear();

        unsigned char byte; // Most Significant Byte
        worldFile.read((char *)&byte, 1);

//        if( byte >= 0xe0)
//        {
//            byteArray.append(byte);
//
//            // Another byte exists!
//            worldFile.read((char *)&byte, 1);
//            byteArray.append(byte);
//        }
//        else
//        {
            byteArray.append(byte);
//        }
        worldFile.read((char *)&byte, 1);
        byteArray.append(byte);

        // Add the byte to the array.
        matlabStream << xMax << ","
                       << yMax << ","
                       << zMax << ","
                       << toInt(byteArray)
                       << endl;

        // Roll the position
        int square = 128;
        xMax++;
        if (xMax>=square)
        {
            xMax = 0;
            yMax++;
            if (yMax>=square)
            {
                yMax = 0;
                zMax++;
                if (zMax >=32)
                {
                    qDebug() << "Nope. Not the right dimensions.";
                }
            }
        }
    }
    matlabFile.close();
    worldFile.close();

    qDebug() << "Closed file.";
}



Q_DECLARE_METATYPE(SavesAccess*)

