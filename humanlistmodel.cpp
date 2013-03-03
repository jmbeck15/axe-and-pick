#include "humanlistmodel.h"

#include <QDebug>
#include <sstream>

Human::Human(const QString &name,
                   const QString &profession,
                   const long &id,
                   QObject * parent)
    : ListItem(parent),
      m_name(name),
      m_profession(profession),
      m_id(id)
{
}

void Human::setProfession(QString profession)
{
    // Placeholder for when things are changed.
}

QHash<int, QByteArray> Human::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ProfessionRole] = "profession";
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant Human::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case ProfessionRole:
        return profession();
    case IdRole:
        return (unsigned int)id();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString Human::filterString() const
{
    std::stringstream completeString;

    completeString <<        name().toStdString()
                   << " " << profession().toStdString();
    return (QString(completeString.str().c_str()));
}

HumanListModel::HumanListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void HumanListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case Human::ProfessionRole:
    {
        Human * item = (Human *)find( id );
        item->setProfession(value.toString());
        break;
    }
    default:
        qWarning() << "HumanListModel::setData does not understand what role" << role << "is.";
        break;
    }
}
