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
        // Mask for the quantity bytes
        const unsigned char MASK = 0x3F;

        // Pull in the list of resource assets (name, group, etc)
        QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.txt");

        if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Opened Asset File: " << assetFile.fileName();

            // Clear the resource model
            resourceModel->clear();

            QTextStream assetStream(&assetFile);
            long byteNumber(0);

            while (!resourceFile.atEnd())
            //for( int i=0; i<3; i++ )
            {
                // Grab the next section and decide the size, etc.
                QString resourceString;
                if (!assetStream.atEnd())
                {
                    resourceString = assetStream.readLine();
                }
                else
                {
                    resourceString = "unexpected data,unknown,resource-unknown.svg";
                }

                QStringList assetData;
                assetData = resourceString.split(',');

                // Temporary array to hold the bytes we read from the file.
                quint8 byteArray[4];

                char sigByte; // Most Significant Byte
                resourceFile.read(&sigByte, 1);
                byteNumber++;
                if( (unsigned char)sigByte >= 0xE0)
                {
                    byteArray[2] = (sigByte - 0xE0);

                    // Pull in the middle byte, because they must be processed together.
                    char middleByte;
                    resourceFile.read(&middleByte, 1);
                    byteNumber++;
                    byteArray[1] = (middleByte & MASK);
                }
                else
                {
                    byteArray[2] = 0;
                    byteArray[1] = sigByte - 0xC0;
                }

                char leastByte;
                resourceFile.read(&leastByte, 1);
                byteNumber++;
                byteArray[0] = leastByte & MASK;

                // Build the quantity out of the read bytes.
                quint32 resourceQuantity;
                resourceQuantity = byteArray[0];
                resourceQuantity |= ((byteArray[1] | (byteArray[2]<<6)) -2) << 6;

                // Create a new Resource with the quantity, and use the
                // data from the ResourceAssetList to flush it out.
                resourceModel->appendRow(new Resource(assetData[0],
                                              assetData[1],
                                              assetData[2],
                                              resourceQuantity,
                                              byteNumber));
            }
        }
        else
        {
            qDebug() << "Could not open " << assetFile.fileName();
        }

    }

}

void SavesAccess::saveResourcesToFile()
{
    // Save it again!
    // This is where the storage stuff should go, I think.
}


Q_DECLARE_METATYPE(SavesAccess*)

