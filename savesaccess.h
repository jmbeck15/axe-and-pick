#ifndef SAVESACCESS_H
#define SAVESACCESS_H

#include <QObject>
#include <QFile>
#include <QDir>

#include "savesoverviewlistmodel.h"
#include "resourcelistmodel.h"

class SavesAccess : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString fileName READ fileName)

public:
    explicit SavesAccess(QObject *parent = 0);

    Q_INVOKABLE void loadSavedGame(QString gameName);
    Q_INVOKABLE void saveSavedGame();
private:
    QDir rootSavesDirectory;

public:
    // Functions to convert to/from binary format
    static long toLong(QByteArray bytes);
    static QByteArray toBinary(long value);


    //
    // Saves Overview
    //
public:
    void setSavesOverviewListModel(SavesOverviewListModel * model);
    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE QString getFilePath();
    Q_INVOKABLE void setFilePath(QString path);
    Q_INVOKABLE bool pathIsValid();
    Q_INVOKABLE void loadGamesList();
private:
    QFile savesOverviewFile;
    SavesOverviewListModel * savesOverview;


    //
    // Resource List
    //
public:
    void setResourceListModel(ResourceListModel * model);
private:
    void loadResourcesList();
    void saveResourcesToFile();
    QFile resourceFile;
    ResourceListModel * resourceModel;
    QByteArray resourceExtraData;   // This is for holding the unexpected data

};

#endif // SAVESACCESS_H
