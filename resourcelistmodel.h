#ifndef RESOURCELISTMODEL_H
#define RESOURCELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include "listmodel.h"

// Resources are stored in ResourceListModels
class Resource : public ListItem
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    enum Roles {
        NameRole  = Qt::UserRole + 1,
        TypeRole,
        IconRole,
        QuantityRole,
        IdRole,
        FileOffsetRole,
        FilterStringRole
    };

    Resource(QObject * parent = 0): ListItem(parent){}
    explicit Resource(const QString &name,
                      const QString &type,
                      const QString &icon,
                      const long &quantity,
                      const long &fileOffset,
                      QObject * parent = 0);

    inline long id() const { return m_fileOffset; }
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setQuantity(long quantity);
    inline QString name() const { return m_name; }
    inline QString type() const { return m_type; }
    inline QString icon() const { return m_icon; }
    inline long quantity() const { return m_quantity; }
    inline long fileOffset() const { return m_fileOffset; }
    QString filterString() const;

private:
    QString m_name;
    QString m_type;
    QString m_icon;
    long m_quantity;
    long m_fileOffset;
};

// ResourceListModels store Resources
class ResourceListModel : public ListModel
{
public:
    ResourceListModel(ListItem * prototype, QObject * parent = 0);

    Q_INVOKABLE void setData(const long identification, const QVariant &value, int role = Resource::QuantityRole);


};

#endif // RESOURCELISTMODEL_H
