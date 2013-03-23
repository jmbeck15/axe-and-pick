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
    //saveUnitsToFile();
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

QByteArray SavesAccess::toBinary(long value)
{
    QByteArray bytes;

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
long SavesAccess::toLong(QByteArray bytes)
{
    // Ensure the bytes have only their relevant bits visible.
    long value;
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
            long index(0);

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
                                          toLong(byteArray), // resource quantity
                                          index++));         // id
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
            QByteArray rawLevelString( unitData[5].toStdString().c_str() );

            QByteArray numberInBytes;
            numberInBytes.clear();

            unsigned char byte(0);
            unsigned int byteNumber(0);
            QList<unsigned int> levels;
            levels.clear();


            // For each job, compute the experience level.
            // NOTE: There are 12 professions and associated levels.
            for (int job=0; job<12; job++)
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
                levels.append(toLong(numberInBytes));

                numberInBytes.clear();
            }
            byteNumber = 0;

            humanModel->appendRow(new Human(i,
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

                                      toLong(unitData[6].toStdString().c_str()), // experience

                                      unitData[7].compare("True") ? false : true,
                                      unitData[8].compare("True") ? false : true,
                                      unitData[9].compare("True") ? false : true,
                                      unitData[10].compare("True") ? false : true,
                                      unitData[11].compare("True") ? false : true,
                                      unitData[12].compare("True") ? false : true,

                                      unitData[13].toFloat(), // rotation

                                      toLong(unitData[14].toStdString().c_str()),
                                      toLong(unitData[15].toStdString().c_str()),
                                      toLong(unitData[16].toStdString().c_str()),
                                      toLong(unitData[17].toStdString().c_str()),
                                      toLong(unitData[18].toStdString().c_str()),

                                      toLong(unitData[19].toStdString().c_str()),

                                      unitData[20].compare("True") ? false : true,
                                      unitData[21].compare("True") ? false : true
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

            neutralMobModel->appendRow(new NeutralMob( i,
                                      unitData[0],
                                      unitData[1].toFloat(),
                                      unitData[2].toFloat(),
                                      unitData[3].toFloat(),
                                      unitData[4].toFloat()) );

            ((NeutralMob *)neutralMobModel->find(i))->print();
        }

        // Load in all the Violent Mobs
        int numberOfViolentMobs = unitStream.readLine().toInt();
        qDebug() << "Loaded" << numberOfViolentMobs << "violent mobs";
        for (int i=0; i<numberOfViolentMobs; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            violentMobModel->appendRow(new ViolentMob( i,
                                      unitData[0],
                                      unitData[1].toFloat(),
                                      unitData[2].toFloat(),
                                      unitData[3].toFloat(),
                                      unitData[4].toFloat()) );

            ((ViolentMob *)violentMobModel->find(i))->print();
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
    if (!unitFile.open(QFile::ReadWrite))
    {
        // TODO: Make this error apparent on the interface somehow.
        // Perhaps by making the save icon be red instead of green.
        qDebug() << "Can't open unitFile for writing.";
        return;
    }
    else
    {
        // Save the numans, then the neutral mobs, then the violent mobs.
        QString unitString;

    }
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
                       << toLong(byteArray)
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

