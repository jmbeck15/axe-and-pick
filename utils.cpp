#include "utils.h"

Utils::Utils()
{
}

QByteArray Utils::toBinary(unsigned int value)
{
    QByteArray bytes;
    bytes.clear();

    // NOTE: I do not understand this. I understand what it's doing, but
    // I have no idea why. I suspect it's much more simple, and I've
    // missed something.
    if (value>1919) // this is the largest two-byte number
    {
        bytes.append( (((value>>6) + 2)>>6) | 0xe0 );
        bytes.append( (((value>>6) + 2) &0x3f) | 0x80 );
    }
    else
    {
        bytes.append( (((value>>6) &0x1f) + 2) | 0xc0 );
    }
    bytes.append((value&0x3f) | 0x80);

    return bytes;
}

// This format handling can certainly be simplified. There's little
// chance that the game designer made it complicated on purpose.
//
unsigned int Utils::toInt(QByteArray bytes)
{
    // Ensure the bytes have only their relevant bits visible.
    unsigned int value;
    if( bytes.size() > 2 )
    {
        bytes[0] = bytes[0] & 0x1f; // Two or more bytes exist, so this one has an extra id bit.
        bytes[1] = bytes[1] & 0x3f;
        bytes[2] = bytes[2] & 0x3f;

        // Compile the bytes into a long.
        //
        // Note: The -2 in the equation is strange. I have no answer as to why
        // this format requires that.
        value = bytes[2] | (((bytes[1] | (bytes[0]<<6)) -2) << 6);
    }
    else
    {
        bytes[0] = bytes[0] & 0x3f;
        bytes[1] = bytes[1] & 0x3f;
        value = bytes[1] | (bytes[0] -2) << 6;
    }

    return value;
}
