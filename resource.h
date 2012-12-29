#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>
#include "listmodel.h"


class Resource : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        NameRole  = Qt::UserRole + 1,
        TypeRole,
        IconRole,
        QuantityRole,
        FileOffsetRole,
        FilterStringRole
    };

public:
    Resource(QObject * parent = 0): ListItem(parent){}
    explicit Resource(const QString &name,
                      const QString &type,
                      const QString &icon,
                      const long &quantity,
                      const long &fileOffset,
                      QObject * parent = 0);

    QVariant data(int role) const;


    void setQuantity(long quantity);

    inline QString id() const { return m_name; }
    inline QString name() const { return m_name; }
    inline QString type() const { return m_type; }
    inline QString icon() const { return m_icon; }
    inline long quantity() const { return m_quantity; }
    inline long fileOffset() const { return m_fileOffset; }
    QString filterString() const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QString m_name;
    QString m_type;
    QString m_icon;
    long m_quantity;
    long m_fileOffset;

};


#endif // RESOURCE_H
