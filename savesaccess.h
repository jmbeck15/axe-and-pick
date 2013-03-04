#ifndef SAVESACCESS_H
#define SAVESACCESS_H

#include <QObject>
#include <QFile>
#include <QDir>

#include "savedgamelistmodel.h"
#include "resourcelistmodel.h"
#include "humanlistmodel.h"
#include "neutralmoblistmodel.h"
#include "violentmoblistmodel.h"

class SavesAccess : public QObject
{
    Q_OBJECT

public:
    explicit SavesAccess(QObject *parent = 0);

    Q_INVOKABLE void loadSavedGame(QString gameName);
    Q_INVOKABLE void saveSavedGame();

    // Functions to convert to/from binary format
    static long toLong(QByteArray bytes);
    static QByteArray toBinary(long value);
private:
    QDir rootSavesDirectory;
    QString selectedSaveName;


    //
    // Saved Game List
    //
public:
    void setSavedGameListModel(SavedGameListModel * model);
    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE QString getSavesPath();
    Q_INVOKABLE void setFilePath(QString path);
    Q_INVOKABLE bool pathIsValid();
    Q_INVOKABLE void loadGamesList();
private:
    SavedGameListModel * savedGameModel;


    //
    // Resource List
    //
public:
    void setResourceListModel(ResourceListModel * model);
private:
    void loadResourceFile();
    void saveResourceFile();

    ResourceListModel * resourceModel;

    QByteArray resourceExtraData;   // This is for holding the unexpected data


    //
    // Unit Lists
    //
public:
    void setHumanModel(HumanListModel * model);
    void setNeutralMobModel(NeutralMobListModel * model);
    void setViolentMobModel(ViolentMobListModel * model);

    void loadUnitFile();
    void saveUnitFile();

private:
    HumanListModel * humanModel;
    NeutralMobListModel * neutralMobModel;
    ViolentMobListModel * violentMobModel;

};

#endif // SAVESACCESS_H
