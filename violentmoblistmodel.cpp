#include "violentmoblistmodel.h"

#include <QDebug>
#include <sstream>
#include <QStringList>

// Initialize the counter
long ViolentMob::id_counter = 0;

ViolentMob::ViolentMob(const QString &type,
                       const float &posX,
                       const float &posY,
                       const float &posZ,
                       const float &rotation,
                       const float &health,
                       QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_type(type),
      m_posX(posX),
      m_posY(posY),
      m_posZ(posZ),
      m_rotation(rotation),
      m_health(health)
{
    id_counter++;
}

ViolentMob * ViolentMob::build(QStringList & unitData)
{
    if (unitData.size() == 5)
    {
        qDebug() << "Version 0.5";
        return (new ViolentMob( unitData[0],
                                unitData[1].toFloat(),
                                unitData[2].toFloat(),
                                unitData[3].toFloat(),
                                unitData[4].toFloat(),
                                0) );
    } else
    if (unitData.size() == 6)
    {
        qDebug() << "Version 0.7";
        return (new ViolentMob( unitData[0],
                                unitData[1].toFloat(),
                                unitData[2].toFloat(),
                                unitData[3].toFloat(),
                                unitData[4].toFloat(),
                                unitData[5].toFloat()) );
    }

    qDebug() << "Error! This Violent Mob version is not supported!";
    return Q_NULLPTR;
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
    names[HealthRole] = "health";

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
    case HealthRole:
        return health();
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
             << "rotation" << rotation()
             << "health" << health();
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

// This function only exists because we can't parse the map file.
// TODO: change how units are placed.
float ViolentMobListModel::getFirstPosition(const char label)
{
    if (getList().size() != 0) {
        if (label==0) {
            return ((ViolentMob*)getList().first())->posX();
        }
        else if (label==1) {
            return ((ViolentMob*)getList().first())->posY();
        }
        else if (label==2) {
            return ((ViolentMob*)getList().first())->posZ();
        }
    }
    return 0.0;
}

void ViolentMobListModel::add(const QString type, float x, float y, float z)
{
    appendRow(new ViolentMob(
                  type,
                  x, y, z,  // position
                  0.0,      // rotation
                  35));     // health

    qDebug() << "Added a mob of type" << type;
}


