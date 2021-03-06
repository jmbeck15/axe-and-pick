/*
 * Author: Christophe Dumez <dchris@gmail.com>
 * License: Public domain (No attribution required)
 * Website: http://cdumez.blogspot.com/
 * Version: 1.0
 */

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

class ListItem: public QObject {
  Q_OBJECT

public:
  ListItem(QObject* parent = 0) : QObject(parent) {}
  virtual ~ListItem() {}

  virtual long id() const = 0;
  virtual QVariant data(int role) const = 0;
  virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
  void dataChanged();
};

class ListModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit ListModel(ListItem* prototype, QObject* parent = 0);
  ~ListModel();
  Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  void appendRow(ListItem* item);
  void appendRows(const QList<ListItem*> &items);
  void insertRow(int row, ListItem* item);
  bool removeRow(int row, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
  ListItem * takeRow(int row);
  ListItem * find(const long &id) const;
  QModelIndex indexFromItem( const ListItem* item) const;
  void clear();
  QList<ListItem*> & getList() { return m_list; }

  // Set this in the inherited model.
  Q_INVOKABLE virtual void setData(const long id, const QVariant &value, int role = Qt::EditRole) = 0;

protected:
  QHash<int, QByteArray> roleNames() const;

private slots:
  void handleItemChange();

private:
  ListItem* m_prototype;
  QList<ListItem*> m_list;
};

#endif // LISTMODEL_H
