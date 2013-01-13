#ifndef SAVESFILE_H
#define SAVESFILE_H

#include <QFile>

class SavesFile
{
public:

    SavesFile(QString filename);
    void open(QString filename);
    void close();
    void reset();

    QString getName();
    void setName(QString desiredName);

    QString getDate();
    void setDate(QString desiredDate);

    unsigned long getUnitQuantity();
    void setUnitQuantity(unsigned long quantity);

    unsigned long getDay();
    void setDay(unsigned long desired_day);

private:
    QFile save_file;

};

#endif // SAVESFILE_H
