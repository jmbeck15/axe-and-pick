#include "savesaccess.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent),
    savesOverview(Q_NULLPTR),
    resourceModel(Q_NULLPTR)
{
}

void SavesAccess::loadSavedGame(QString gameName)
{
    // Load the game!


    resourceFile.setFileName(rootSavesDirectory.absolutePath()
                             + "/" + gameName
                             + "/" + "re.sav");
    qDebug() << "Game to load" << resourceFile.fileName();
    loadResourcesList();
}

void SavesAccess::saveSavedGame()
{
    // Save the game!
    saveResourcesToFile();
}

void SavesAccess::openFileDialog()
{

    // TODO: Set the parent of this dialog so it returns propery.
    // Not sure if this is the cause of the weird Windows behavior
    // or not. Something to try.
    QFileDialog fileDialog(0,"Timber and Stone saves.sav file");
    fileDialog.setDirectory(rootSavesDirectory);
    fileDialog.setNameFilter("Saves File (saves.sav)");

    if (fileDialog.exec())
    {
        rootSavesDirectory.setPath(fileDialog.selectedFiles().first());
        rootSavesDirectory.cdUp(); // This trims off the saves.sav file.

        savesOverviewFile.setFileName(rootSavesDirectory.absolutePath()
                                      + "/" + "saves.sav");
    }
}

void SavesAccess::setFilePath(QString path)
{
    rootSavesDirectory.setPath(path);
    rootSavesDirectory.cdUp();

    savesOverviewFile.setFileName(path);
}

QString SavesAccess::getFilePath()
{
    return savesOverviewFile.fileName();
}

bool SavesAccess::pathIsValid()
{
    // Return True if both the directory path is valid
    // and the saves.sav file exists.
    QFileInfo fileInfo(savesOverviewFile);
    if (savesOverviewFile.exists() && (fileInfo.fileName() == "saves.sav"))
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
    if (savesOverview == Q_NULLPTR)
    {
        qDebug() << "SavedAccess model hasn't been set up yet.";
        return;
    }

    // Open file and make sure it went okay.
    if (!savesOverviewFile.exists() || !savesOverviewFile.open(QFile::ReadOnly | QFile::Text))
    {
//        QMessageBox::warning(this, tr("Application"),
//                                      tr("Cannot read savesOverviewFile %1:\n%2.")
//                                      .arg(savesOverviewFile.savesOverviewFile())
//                                      .arg(savesOverviewFile.errorString()));
        qDebug() << "Can't open savesOverviewFile.";
        return;
    }
    else
    {

        QTextStream in(&savesOverviewFile);
        QStringList strings;

        long numberOfGames = in.readLine().toLongLong();
        qDebug() << "Loaded" << numberOfGames << "games.";

        // Remove all the games.
        savesOverview->clear();

        // Add the games from the saves.sav file.
        while (!in.atEnd())
        {
            strings = in.readLine().split("/", QString::KeepEmptyParts);

            savesOverview->appendRow(new SavesOverviewItem(strings[0],
                                 strings[3],
                                 strings[4],
                                 strings[1].toLongLong(),
                                 strings[2].toLongLong()));
        }
    }
    savesOverviewFile.close();

}

void SavesAccess::setSavesOverviewListModel(SavesOverviewListModel * model)
{
    savesOverview = model;
}


void SavesAccess::setResourceListModel(ResourceListModel * model)
{
    resourceModel = model;
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
//        QMessageBox::warning(this, tr("Application"),
//                                      tr("Cannot read savesOverviewFile %1:\n%2.")
//                                      .arg(savesOverviewFile.savesOverviewFile())
//                                      .arg(savesOverviewFile.errorString()));
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


                // TODO: The format may call for a fourth byte, with a
                // leading FOUR bits instead of three. This should be easy
                // to impliment, but I haven't verified that it really exists.
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

                // Create a new Resource with the quantity, and use the
                // data from the ResourceAssetList to flush it out.
                //
                // Check to see if the resource is unknown
//                if (assetData[0].compare("unknown"))
//                {
                    resourceModel->appendRow(new Resource(assetData[0],
                                              assetData[1],
                                              assetData[2],
                                              toLong(byteArray), // resource quantity
                                              index++));
//                }
//                else
//                {
//                    // Unknown resource! Hide it by marking it with a tick mark.
//                    // TODO: This should be done better somehow.
//                    resourceModel->appendRow(new Resource(assetData[0],
//                                              assetData[1],
//                                              assetData[2],
//                                              toLong(byteArray), // resource quantity
//                                              index++));
//                }
            }

            if (!resourceFile.atEnd())
            {
                // Read the rest of the file into an array, and keep it for later.
                resourceExtraData.clear();
                resourceExtraData = resourceFile.readAll();
                //qDebug() << resourceExtraData.toHex();
            }
        }
        else
        {
            qDebug() << "Could not open " << assetFile.fileName();
        }

        assetFile.close();
        resourceFile.close();
    }

}


void SavesAccess::saveResourcesToFile()
{
    // This is where the storage stuff should go, I think.

    qDebug() << "You want the resources saved, eh? Okay.";

    if (resourceModel == Q_NULLPTR
            || resourceModel->rowCount() <= 0)
    {
        qDebug() << "The resource model hasn't been populated.";
        return;
    }

    // Open file for write, and make sure it went okay.
    if (!resourceFile.open(QFile::ReadWrite))
    {
//        QMessageBox::warning(this, tr("Application"),
//                                      tr("Cannot read savesOverviewFile %1:\n%2.")
//                                      .arg(savesOverviewFile.savesOverviewFile())
//                                      .arg(savesOverviewFile.errorString()));
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
                //
                // TODO: I don't know why this doesn't emit the signal that
                // QML reads to update the view. setData() does emit...
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


Q_DECLARE_METATYPE(SavesAccess*)

