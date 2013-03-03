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

void SavesAccess::loadSavedGame(QString gameName)
{
    // Build the file name.
    resourceFile.setFileName(rootSavesDirectory.absolutePath()
                             + "/" + gameName
                             + "/" + "re.sav");
    // Pull the resources into the list.
    loadResourcesList();
}

void SavesAccess::saveSavedGame()
{
    saveResourcesToFile();
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

        savedGameFile.setFileName(rootSavesDirectory.absolutePath()
                                      + "/" + "saves.sav");
    }
}


// TODO: there are two areas which save the paths; this one and the one
// above in the file dialog area. Combine these.
void SavesAccess::setFilePath(QString path)
{
    // Save the path to the file.
    rootSavesDirectory.setPath(path);
    rootSavesDirectory.cdUp();

    // Save the file name.
    savedGameFile.setFileName(path);
}

QString SavesAccess::getFilePath()
{
    return savedGameFile.fileName();
}

bool SavesAccess::pathIsValid()
{
    // Return True if both the directory path is valid
    // and the saves.sav file exists.
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
    bytes.append(value&0x3f | 0x80);

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

void SavesAccess::loadResourcesList()
{
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


void SavesAccess::saveResourcesToFile()
{
    if (resourceModel == Q_NULLPTR
            || resourceModel->rowCount() <= 0)
    {
        qDebug() << "The resource model hasn't been populated.";
        return;
    }

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
// HUMANS
// =====================

void SavesAccess::setHumanListModel(HumanListModel * model)
{
    humanModel = model;
}

Q_DECLARE_METATYPE(SavesAccess*)
