#include "savesaccess.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent),
    savesOverview(Q_NULLPTR)
{
}

void SavesAccess::openFileDialog()
{

    // TODO: Set the parent of this dialog so it returns propery.
    // Not sure if this is the cause of the weird Windows behavior
    // or not. Something to try.
    QFileDialog fileDialog(0,"Timber and Stone saves.sav file");
    fileDialog.setDirectory(file.fileName());
    fileDialog.setNameFilter("Saves File (saves.sav)");

    if (fileDialog.exec())
    {
        file.setFileName(fileDialog.selectedFiles().first());
    }
}

void SavesAccess::setFilePath(QString path)
{
    file.setFileName(path);
}

QString SavesAccess::getFilePath()
{
    return file.fileName();
}

bool SavesAccess::pathIsValid()
{
    // Return True if both the directory path is valid
    // and the saves.sav file exists.
    QFileInfo fileInfo(file);
    if (file.exists() && (fileInfo.fileName() == "saves.sav"))
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
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text))
    {
//        QMessageBox::warning(this, tr("Application"),
//                                      tr("Cannot read file %1:\n%2.")
//                                      .arg(file.fileName())
//                                      .arg(file.errorString()));
        qDebug() << "Can't open file.";
        return;
    }
    else
    {

        QTextStream in(&file);
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
    file.close();

}

void SavesAccess::setSavesOverviewListModel(SavesOverviewListModel * model)
{
    savesOverview = model;
}

Q_DECLARE_METATYPE(SavesAccess*)

