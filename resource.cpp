
#include <QDebug>
#include <sstream>
#include "resource.h"


Resource::Resource(const QString &name,
                   const QString &type,
                   const QString &icon,
                   const long &quantity,
                   const long &fileOffset,
                   QObject * parent)
    : ListItem(parent),
      m_name(name),
      m_type(type),
      m_icon(icon),
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
    names[IconRole] = "icon";
    names[QuantityRole] = "quantity";
    names[FileOffsetRole] = "fileOffset";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant Resource::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case TypeRole:
        return type();
    case IconRole:
        return icon();
    case QuantityRole:
        return quantity();
    case FileOffsetRole:
        return fileOffset();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString Resource::filterString() const
{
    std::stringstream completeString;

    completeString << "name:" << name().toStdString()
                   << " type:" << type().toStdString()
                   << " quantity:" << quantity();
    return (QString(completeString.str().c_str()));
}
