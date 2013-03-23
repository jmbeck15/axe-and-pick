#include "violentmoblistmodel.h"

#include <QDebug>
#include <sstream>

// Initialize the counter
long ViolentMob::id_counter = 0;

ViolentMob::ViolentMob(const QString &type,
                       const float &posX,
                       const float &posY,
                       const float &posZ,
                       const float &rotation,
                       QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_type(type),
      m_posX(posX),
      m_posY(posY),
      m_posZ(posZ),
      m_rotation(rotation)
{
    id_counter++;
}

void ViolentMob::setType(QString type)
{
    m_type = type;
}

QHash<int, QByteArray> ViolentMob::roleNames() const
{
    QHash<int, QByteArray> names;

    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    names[TypeRole] = "type";
    names[PosXRole] = "posX";
    names[PosYRole] = "posY";
    names[PosZRole] = "posZ";
    names[RotationRole] = "rotation";

    return names;
}

QVariant ViolentMob::data(int role) const
{
    switch(role) {
    case IdRole:
        return (unsigned int)id();
    case FilterStringRole:
        return filterString();

    case TypeRole:
        return type();
    case PosXRole:
        return posX();
    case PosYRole:
        return posY();
    case PosZRole:
        return posZ();
    case RotationRole:
        return rotation();
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

void ViolentMob::print()
{
    qDebug() << type()
             << "x" << posX()
             << "y" << posY()
             << "z" << posZ()
             << "rotation" << rotation();
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

void ViolentMobListModel::remove(const long id)
{
    // Find the id of the item you want to delete,
    // get the index of that item, and remove it.
    removeRow(indexFromItem(find( id )).row());
}
