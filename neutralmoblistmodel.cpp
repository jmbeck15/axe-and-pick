#include "neutralmoblistmodel.h"

#include <QDebug>
#include <sstream>
#include <QStringList>

// Initialize the counter
long NeutralMob::id_counter = 0;

NeutralMob::NeutralMob(const QString &type,
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

NeutralMob * NeutralMob::build(QStringList & unitData)
{
    if (unitData.size() == 6)
    {
        return (new NeutralMob( unitData[0],
                                unitData[1].toFloat(),
                                unitData[2].toFloat(),
                                unitData[3].toFloat(),
                                unitData[4].toFloat()) );
    }

    qDebug() << "Error! This Neutral Mob version is not supported!";
    return Q_NULLPTR;
}

void NeutralMob::setType(QString type)
{
    m_type = type;
}

QHash<int, QByteArray> NeutralMob::roleNames() const
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

QVariant NeutralMob::data(int role) const
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

QString NeutralMob::filterString() const
{
    std::stringstream completeString;

    completeString << type().toStdString();
    return (QString(completeString.str().c_str()));
}

void NeutralMob::print()
{
    qDebug() << type()
             << "x" << posX()
             << "y" << posY()
             << "z" << posZ()
             << "rotation" << rotation();
}

NeutralMobListModel::NeutralMobListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void NeutralMobListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case NeutralMob::TypeRole:
    {
        NeutralMob * item = (NeutralMob *)find( id );
        item->setType(value.toString());
        break;
    }
    default:
        qWarning() << "NeutralMobListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

void NeutralMobListModel::remove(const long id)
{
    // Find the id of the item you want to delete,
    // get the index of that item, and remove it.
    removeRow(indexFromItem(find( id )).row());
}

// This function only exists because we can't parse the map file.
// TODO: change how units are placed.
float NeutralMobListModel::getFirstPosition(const char label)
{
    if (getList().size() != 0) {
        if (label==0) {
            return ((NeutralMob*)getList().first())->posX();
        }
        else if (label==1) {
            return ((NeutralMob*)getList().first())->posY();
        }
        else if (label==2) {
            return ((NeutralMob*)getList().first())->posZ();
        }
    }
    return 0.0;
}

void NeutralMobListModel::add(const QString type, float x, float y, float z)
{
    appendRow(new NeutralMob(
                  type,
                  x, y, z,  // position
                  0.0 ));    // rotation

    qDebug() << "Added a mob of type" << type;
}

