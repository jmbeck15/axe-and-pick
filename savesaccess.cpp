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
        in.setCodec("UTF-8");
        QStringList strings;

        // Read the number of games in the file, but
        // don't do anything with it.
        in.readLine().toLongLong();

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
    if (!resourceFile.exists() || !resourceFile.open(QFile::ReadOnly))
    {
        QString message = "I can't load the re.sav file! Is it missing? Saving is disabled.";
        emit fileLoadStatusChanged(true, message);

        return;
    }
    else
    {

        // Pull in the list of resource assets (name, group, etc)
        QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.csv");

        if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
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
                                          Utils::toInt(byteArray))); // resource quantity
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
            QString message = "I can't load the resource_list.csv file! Is it missing? Saving is disabled.";
            emit fileLoadStatusChanged(true, message);
        }

        assetFile.close();
        resourceFile.close();
    }

}


void SavesAccess::saveResourceFile()
{
    //
    // Ha ha ha! Man, I'm hilarious.
    //
//    if (qrand()%1000 < 10) {
//        // NOTE: This doesn't mess things up. Just save again and chance are
//        // good that it'll save next time.
//        QString message = "Failed to save game. The only loss of resources is one saved game.";
//        emit fileSaveStatusChanged(true, message);

//        return;
//    }

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
    if (!resourceFile.open(QIODevice::WriteOnly))
    {
        QString message = "Saving failed! Check to make sure the re.sav file isn't open in another program.";
        emit fileSaveStatusChanged(true, message);

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
            if (quantity > 60000)
            {
                qDebug() << "Resource quantities above 65,278 will fail to load in the game. Reduced to 60,000.";
                quantity = 60000;

                // Update the resource. setData() requires an "id", not an index.
                resourceModel->setData(ndx+1,60000);
            }
            binaryData = Utils::toBinary(quantity);
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
    bool errorOccured(false);
    QString message;

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
    if (!unitFile.exists() || !unitFile.open(QFile::ReadOnly))
    {
        // TODO: Make this error apparent on the interface somehow.
        QString message = "The unit file (un.sav) seems to be missing! Saving disabled.";
        emit fileLoadStatusChanged(true, message);

        return;
    }
    else
    {
        QTextStream unitStream(&unitFile);

        QString unitString;
        QStringList unitData;

        //
        // Load in all the Humans
        //
        humanModel->clear();
        int numberOfHumans = unitStream.readLine().toInt();
        for (int i=0; i<numberOfHumans; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            Human * newHuman = Human::build(unitData);
            if (newHuman != Q_NULLPTR)
            {
                humanModel->appendRow(newHuman);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Human string (not legal!).";
            }
        }

        //
        // Load in all the Neutral Mobs
        //
        neutralMobModel->clear();
        int numberOfNeutralMobs = unitStream.readLine().toInt();
        for (int i=0; i<numberOfNeutralMobs; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            NeutralMob * newMob = NeutralMob::build(unitData);
            if (newMob != Q_NULLPTR)
            {
                neutralMobModel->appendRow(newMob);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Neutral Mob string (not legal!).";
            }
        }

        //
        // Load in all the Violent Mobs
        //
        violentMobModel->clear();
        int numberOfViolentMobs = unitStream.readLine().toInt();
        for (int i=0; i<numberOfViolentMobs; i++)
        {
            unitString = unitStream.readLine();
            unitData = unitString.split('/');

            ViolentMob * newMob = ViolentMob::build(unitData);
            if (newMob != Q_NULLPTR)
            {
                violentMobModel->appendRow(newMob);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Violent Mob string (not legal!).";
            }
        }

        qDebug() << "From" << selectedSaveName
                << ": Loaded" << humanModel->rowCount() << "of" << numberOfHumans << "humans,"
                 << neutralMobModel->rowCount() << "of" << numberOfNeutralMobs << "animals, and"
                 << violentMobModel->rowCount() << "of" << numberOfViolentMobs << "bad guys.";

        if (errorOccured) {
            message = "This saved-game version is not supported! Saving is disabled and data may be innacurate.";
        } else {
            message = "Game \"" + selectedSaveName + "\" loaded properly.";
        }
        emit fileLoadStatusChanged(errorOccured, message);

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
        qDebug() << "The unit models haven't been set up.";

        return;
    }

    QFile unitFile(rootSavesDirectory.absolutePath()
                       + "/" + selectedSaveName
                       + "/" + "un.sav");


    // Open file for write, and make sure it went okay.
    if (!unitFile.open(QIODevice::WriteOnly))
    {
        QString message = "Saving failed! Check to make sure the un.sav file isn't open in another program.";
        emit fileSaveStatusChanged(true, message);

        return;
    }
    else
    {
        QTextStream unitStream(&unitFile);

        // Save the humans, then the neutral mobs, then the violent mobs.

        // Humans
        unitStream << humanModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = humanModel->getList().begin(); itr != humanModel->getList().end(); itr++)
        {
            Human* human = (Human*)*itr;
            human->writeToFile(unitFile);
        }

        // Neutral Mobs
        unitStream << neutralMobModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = neutralMobModel->getList().begin(); itr != neutralMobModel->getList().end(); itr++)
        {
            NeutralMob* mob = (NeutralMob*)*itr;
            unitStream << mob->type() << "/"
                       << mob->posX() << "/"
                       << mob->posY() << "/"
                       << mob->posZ() << "/"
                       << mob->rotation() << "/";

            // Write all the unknown floats
            for (int i=0; i<4; i++)
            {
                unitStream << mob->unknown_float(i) << "/";
            }

            // Write all the options
            for (int i=0; i<3; i++)
            {
                unitStream << QString(mob->option(i)?"True":"False") << "/";
            }

            unitStream << endl;
        }

        // Violent Mobs
        unitStream << violentMobModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = violentMobModel->getList().begin(); itr != violentMobModel->getList().end(); itr++)
        {
            ViolentMob* mob = (ViolentMob*)*itr;
            unitStream << mob->type() << "/"
                       << mob->posX() << "/"
                       << mob->posY() << "/"
                       << mob->posZ() << "/"
                       << mob->rotation() << "/"
                       << mob->health() << "/"
                       << mob->subtype() << "/"
                       << QString(mob->leader() ? "True" : "False") << "/";
            unitStream << endl;
        }
    }

    QString message = "Saved.";
    emit fileSaveStatusChanged(false, message);

    unitFile.close();
}

void SavesAccess::writeToMatlab(int squareSize)
{
    qDebug() << "Opening file...";

    // Create a new file for storing all these points. Comma seperated.
    QFile matlabFile(rootSavesDirectory.absolutePath()
                    + "/" + selectedSaveName
                    + "/" + "cd.dat");
    matlabFile.open(QFile::WriteOnly);

    QFile worldFile(rootSavesDirectory.absolutePath()
                    + "/" + selectedSaveName
                    + "/" + "cd.sav");
    worldFile.open(QFile::ReadOnly);

    // Go until you get all the blocks
    for( int blockNumber = 0; blockNumber < (squareSize * squareSize * 48); blockNumber++ )
    {
        // Grab a line in the file
        QByteArray lineOfBytes = worldFile.readLine();

        int i;
        i=0;
        while( i < lineOfBytes.length())
        {
            // Get the most significant byte
            unsigned char byte = lineOfBytes.at(i);
            i++;

            // Temporary array to hold the bytes we read from the file.
            QByteArray byteArray;
            byteArray.clear();
            if( byte >= 0xe0)
            {
                byteArray.append(byte);

                // Another byte exists!
                byte = lineOfBytes.at(i);
                i++;
                byteArray.append(byte);
            }
            else
            {
                byteArray.append(byte);
            }
            byte = lineOfBytes.at(i);
            i++;
            byteArray.append(byte);

            // Add the byte to the array.
            if( Utils::toInt(byteArray) != 125
             && Utils::toInt(byteArray) != 126
             && Utils::toInt(byteArray) != 714
             && Utils::toInt(byteArray) != 0 )
            {
                matlabFile.write(Utils::toBinary(1));
            }
            else
            {
                matlabFile.write(byteArray);
            }
        }
        // First Block done!
        //QByteArray newline("\n");
        //matlabFile.write(newline);
    }

    // The remaining lines are numbers.
    QByteArray tempBytes;
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);

    matlabFile.close();
    worldFile.close();

    qDebug() << "Closed file.";
}



Q_DECLARE_METATYPE(SavesAccess*)

