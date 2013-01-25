#include "savesaccess.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent),
    savesOverview(Q_NULLPTR)
{
}

void SavesAccess::loadSavedGame(unsigned long id)
{
    // Load the game!
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
    fileDialog.setDirectory(savesOverviewFile.fileName());
    fileDialog.setNameFilter("Saves File (saves.sav)");

    if (fileDialog.exec())
    {
        savesOverviewFile.setFileName(fileDialog.selectedFiles().first());
    }
}

void SavesAccess::setFilePath(QString path)
{
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


void SavesAccess::loadResourcesList()
{
    // Load it!
}

void SavesAccess::saveResourcesToFile()
{
    // Save it again!
    // This is where the storage stuff should go, I think.
}


Q_DECLARE_METATYPE(SavesAccess*)

