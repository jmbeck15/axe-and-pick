#include "neutralmoblistmodel.h"

#include <QDebug>
#include <sstream>

NeutralMob::NeutralMob( const QString &type,
                        const long &id,
                        QObject * parent)
    : ListItem(parent),
      m_type(type),
      m_id(id)
{
}

void NeutralMob::setType(QString type)
{
    m_type = type;
}

QHash<int, QByteArray> NeutralMob::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TypeRole] = "type";
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant NeutralMob::data(int role) const
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

QString NeutralMob::filterString() const
{
    std::stringstream completeString;

    completeString << type().toStdString();
    return (QString(completeString.str().c_str()));
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
