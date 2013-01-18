#ifndef SAVESACCESS_H
#define SAVESACCESS_H

#include <QObject>
#include <QFile>

class SavesAccess : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString fileName READ fileName)

public:
    explicit SavesAccess(QObject *parent = 0);

    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE void setFileName(QString name);

private:
    QFile file;
    QString fileName;
    // List model goes here (or maybe public, I'm not sure)

};

#endif // SAVESACCESS_H
