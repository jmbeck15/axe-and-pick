
#include <QDebug>
#include "resource.h"

Resource::Resource(const QString &name, const long &quantity, const long &fileOffset, QObject * parent)
    : ListItem(parent),
      m_name(name),
      m_quantity(quantity),
      m_fileOffset(fileOffset)
{
}

void Resource::setQuantity(long quantity)
{
    if(m_quantity != quantity)
    {
        m_quantity = quantity;
        emit dataChanged();
    }
}

QHash<int, QByteArray> Resource::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[TypeRole] = "type";
    names[QuantityRole] = "quantity";
    names[FileOffsetRole] = "fileOffset";
    qDebug() << "That worked just fine.";
    return names;
}

QVariant Resource::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case TypeRole:
        return type();
    case QuantityRole:
        return quantity();
    case FileOffsetRole:
        return fileOffset();
    default:
        return QVariant();
    }
}
