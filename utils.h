#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>

class Utils
{
public:
    Utils();

    // Functions to convert to/from binary format
    static unsigned int toInt(QByteArray bytes);
    static QByteArray toBinary(unsigned int value);
};

#endif // UTILS_H
