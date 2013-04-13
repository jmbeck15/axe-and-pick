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
                       const QList<float> &unknown_floats,
                       const QBitArray &options,
                       QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_type(type),
      m_posX(posX),
      m_posY(posY),
      m_posZ(posZ),
      m_rotation(rotation),
      m_unknown(unknown_floats),
      m_options(options)
{
    id_counter++;
}

NeutralMob * NeutralMob::build(QStringList & unitData)
{
    if (unitData.size() == 13)
    {
        // Load in the unknown floats
        QList<float> unknown_floats;
        for (int i=0; i<4; i++)
        {
            unknown_floats.append(unitData[i+5].toFloat());
        }

        // Load in the options
        QBitArray options(3, false);
        for (int i=0; i<3; i++)
        {
            options.setBit(i, unitData[i+9].compare("True") ? false : true);
        }

        return (new NeutralMob( unitData[0],
                                unitData[1].toFloat(),
                                unitData[2].toFloat(),
                                unitData[3].toFloat(),
                                unitData[4].toFloat(),
                                unknown_floats,
                                options ));
    }

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
    QList<float> unknown_floats;
    unknown_floats.append(0.4f);
    unknown_floats.append(0.0f);
    unknown_floats.append(0.0f);
    unknown_floats.append(0.0f);

    QBitArray options(3, false);

    appendRow(new NeutralMob(
                  type,
                  x, y, z,  // position
                  0.0,      // rotation
                  unknown_floats,
                  options) );

    qDebug() << "Added a mob of type" << type;
}

