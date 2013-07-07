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
                       const float &subtype,
                       const bool  &leader,
                       QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_type(type),
      m_posX(posX),
      m_posY(posY),
      m_posZ(posZ),
      m_rotation(rotation),
      m_health(health),
      m_subtype(subtype),
      m_leader(leader)
{
    id_counter++;
}

ViolentMob * ViolentMob::build(QStringList & unitData)
{
    if (unitData.size() == 9)
    {
        return (new ViolentMob( unitData[0],
                                unitData[1].toFloat(),
                                unitData[2].toFloat(),
                                unitData[3].toFloat(),
                                unitData[4].toFloat(),
                                unitData[5].toFloat(),
                                unitData[6].toFloat(),
                                unitData[7].compare("True") ? false : true));
    }

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
    names[SubtypeRole] = "subtype";

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
    case SubtypeRole:
        return subtype();
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

void ViolentMobListModel::add(const QString type, int subtype, float x, float y, float z)
{
    // Randomly choose leader or not (about 5 percent
    // of the bad guys are leaders)
    int high = 100;
    int low = 0;
    bool isALeader = ((qrand() % ((high + 1) - low) + low) > 95) ? true : false;

    // Randomly choose a rotation, so they aren't all
    // facing the same way. (hopefully better move dynamics?)
    high = 180;
    low = 0;
    float rotation = qrand() % ((high + 1) - low) + low;

    appendRow(new ViolentMob(
                  type,
                  x, y, z,     // position
                  rotation,    // rotation
                  100,         // health
                  subtype,        // type of the type of mob
                  isALeader)); // leader


    qDebug() << "Added a" << type << "mob of type" << subtype;
}


