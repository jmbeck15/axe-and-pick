#include "savesaccess.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent)
{
}

void SavesAccess::openFileDialog()
{

    qDebug() << "Finished directory...";

#if 0

    QString tempPath =
            QFileDialog::getOpenFileName(0,
                                         "Timber and Stone installation directory",
                                         filePath.absolutePath(),
                                         "Saves File (saves.sav)");
#endif

#if 1
    QString tempPath;
    QFileDialog fileDialog(0,"Timber and Stone saves.sav file");
    qDebug() << "Created dialog...";
    fileDialog.setDirectory(filePath);
    qDebug() << "Set start directory...";
    fileDialog.setNameFilter("Saves File (saves.sav)");
    qDebug() << "Set filter...";

    if (fileDialog.exec())
    {
        qDebug() << "Selected a file...";
        filePath.setPath(fileDialog.selectedFiles().first());
        qDebug() << "Path: " << filePath.absolutePath();
    }
    qDebug() << "Done.";
#endif


    if (!tempPath.isNull())
    {
        // The path isn't blank, so consider it valid.
        // TODO: Check to make sure the saves.sav file exists somewhere
        // relative to the selected file. If not, discard the data and
        // alert someone.
        filePath = tempPath;
    }
    else
    {
        // The operation was canceled; no file selected.
    }
}

void SavesAccess::setFilePath(QString path)
{
    filePath.setPath(path);
}

QString SavesAccess::getFilePath()
{
    return filePath.absolutePath();
}

Q_DECLARE_METATYPE(SavesAccess*)

