#include "resourcelistmodel.h"

#include <QDebug>
#include <sstream>

// Initialize the counter
long Resource::id_counter = 0;

Resource::Resource(const QString &name,
                   const QString &type,
                   const QString &icon,
                   const long &quantity,
                   QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_name(name),
      m_type(type),
      m_icon(icon),
      m_quantity(quantity)
{
    id_counter++;
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
    names[IdRole] = "id";
    names[NameRole] = "name";
    names[TypeRole] = "type";
    names[IconRole] = "icon";
    names[QuantityRole] = "quantity";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant Resource::data(int role) const
{
    switch(role) {
    case IdRole:
        return (unsigned int)id();
    case NameRole:
        return name();
    case TypeRole:
        return type();
    case IconRole:
        return icon();
    case QuantityRole:
        return (unsigned int)quantity();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString Resource::filterString() const
{
    std::stringstream completeString;

    completeString <<        name().toStdString()
                   << " " << type().toStdString()
                   << " " << quantity();
    return (QString(completeString.str().c_str()));
}

ResourceListModel::ResourceListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void ResourceListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case Resource::QuantityRole:
    {
        Resource * item = (Resource *)find(id);
        item->setQuantity(value.toLongLong());
        break;
    }
    default:
        qWarning() << "ResourceListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

void ResourceListModel::setUnknownQuantities()
{
    long counter = 0;
    for (QList<ListItem*>::iterator itr = getList().begin(); itr != getList().end(); itr++)
    {
        if(((Resource*)*itr)->name().compare("unknown") == 0)
        {
            ((Resource*)*itr)->setQuantity(counter+1000);
        }
        counter++;
    }

    qDebug() << "Set unknown quantities. Remember to save.";
}
