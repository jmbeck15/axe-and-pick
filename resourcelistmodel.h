#ifndef RESOURCELISTMODEL_H
#define RESOURCELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// Resources are stored in ResourceListModels
class Resource : public ListItem
{
private:
    Q_OBJECT
    Q_ENUMS(Roles)

    static long id_counter;

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        FilterStringRole,
        NameRole,
        TypeRole,
        QuantityRole
    };

    Resource(QObject * parent = 0): ListItem(parent){id_counter++;}
    explicit Resource(const QString &name,
                      const QString &type,
                      const long &quantity,
                      QObject * parent = 0);

    inline long id() const { return m_id; }
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setQuantity(long quantity);
    inline QString name() const { return m_name; }
    inline QString type() const { return m_type; }
    inline long quantity() const { return m_quantity; }
    QString filterString() const;

private:
    long m_id;
    QString m_name;
    QString m_type;
    long m_quantity;

};

// ResourceListModels store Resources
class ResourceListModel : public ListModel
{
private:
    Q_OBJECT

public:
    ResourceListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long identification, const QVariant &value, int role = Resource::QuantityRole);

    // Utilities
    Q_INVOKABLE void setQuantitiesInOrder();

};

#endif // RESOURCELISTMODEL_H
