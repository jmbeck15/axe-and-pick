#include "resourcelistmodel.h"

#include <QDebug>
#include <sstream>

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
    names[IdRole] = "identification";
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
    case IdRole:
        return id();
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

ResourceListModel::ResourceListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void ResourceListModel::setData(const long identification, const QVariant &value, int role)
{
    switch (role)
    {
    case Resource::QuantityRole:
    {
        Resource * item = (Resource *)find(identification);
        item->setQuantity(value.toLongLong());
        break;
    }
    default:
        qWarning() << "ResourceListModel::setData does not understand what role" << role << "is.";
        break;
    }
}
