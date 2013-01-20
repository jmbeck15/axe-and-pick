#ifndef SAVESACCESS_H
#define SAVESACCESS_H

#include <QObject>
#include <QFile>
#include <QDir>

class SavesAccess : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString fileName READ fileName)

public:
    explicit SavesAccess(QObject *parent = 0);

    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE QString getFilePath();
    Q_INVOKABLE void setFilePath(QString path);
    Q_INVOKABLE bool pathIsValid();
    Q_INVOKABLE void loadGamesList();

private:
    QFile file;
    //SavedGamesListModel savedGames;

};

#endif // SAVESACCESS_H
