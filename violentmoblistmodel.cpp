#include "violentmoblistmodel.h"

#include <QDebug>
#include <sstream>

ViolentMob::ViolentMob( const QString &type,
                        const long &id,
                        QObject * parent)
    : ListItem(parent),
      m_type(type),
      m_id(id)
{
}

void ViolentMob::setType(QString type)
{
    m_type = type;
}

QHash<int, QByteArray> ViolentMob::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TypeRole] = "type";
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant ViolentMob::data(int role) const
{
    switch(role) {
    case TypeRole:
        return type();
    case IdRole:
        return (unsigned int)id();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString ViolentMob::filterString() const
{
    std::stringstream completeString;

    completeString << type().toStdString();
    return (QString(completeString.str().c_str()));
}

ViolentMobListModel::ViolentMobListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void ViolentMobListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case ViolentMob::TypeRole:
    {
        ViolentMob * item = (ViolentMob *)find( id );
        item->setType(value.toString());
        break;
    }
    default:
        qWarning() << "ViolentMobListModel::setData does not understand what role" << role << "is.";
        break;
    }
}
