#ifndef KVJSONSTORE_H
#define KVJSONSTORE_H

#include <QObject>
#include <QList>
#include <QSet>
#include <QString>
#include <QVariant>

/**
 * Manager for managing lists of JSON data.
 */
class KVJsonStore : QObject
{
	Q_OBJECT
	
	/**
	 * If an ID key is given, it will be used to guarantee uniqueness.
	 * 
	 * Re-inserts with the same ID will be ignored.
	 */
	Q_PROPERTY(QString idKey READ idKey WRITE setIdKey NOTIFY idKeyChanged);
	
	/**
	 * If a sort key is given, the list is kept sorted by it.
	 * 
	 * If this is blank, new items will simply be inserted at the bottom.
	 * 
	 * Setting this property will cause the data to be re-sorted, which can
	 * potentially be expensive if the dataset is large.
	 */
	Q_PROPERTY(QList<QString> sortKeys READ sortKeys WRITE setSortKeys NOTIFY sortKeysChanged);
	
	/// A list of all inserted IDs
	Q_PROPERTY(const QSet<QString>& ids READ ids NOTIFY idsChanged);
	
public:
	/**
	 * Constructor.
	 */
	KVJsonStore();
	
	/**
	 * Destructor.
	 */
	virtual ~KVJsonStore();
	
	QString idKey() const;						///< Getter for idKey
	void setIdKey(QString v);					///< Setter for idKey
	
	QList<QString> sortKeys() const;			///< Getter for sortKeys
	void setSortKeys(QList<QString> v);			///< Setter for sortKeys
	
	const QSet<QString>& ids() const;			///< Getter for ids
	const QList<QVariant>& data() const;		///< Getter for data
	
	/**
	 * Inserts a new object into the store.
	 */
	void add(const QVariant &v);
	
signals:
	void idKeyChanged(QString v);				///< Emitted when idKey changes
	void sortKeysChanged(QList<QString> v);		///< Emitted when sortKeys changes
	void idsChanged(const QSet<QString>& v);	///< Emitted when ids changes
	
protected:
	/**
	 * Rebuild the ids() set.
	 * 
	 * Does nothing unless idKey is set.
	 */
	void rebuildIds();
	
	/**
	 * Adds the ID for the given item to the ids() set.
	 * 
	 * Does nothing unless idKey is set.
	 */
	void addIdFor(const QVariant &v);
	
	/**
	 * Sorts the data() list.
	 * 
	 * Does nothing unless sortKeys is set.
	 */
	void sortData();
	
private:
	QString m_idKey;
	QList<QString> m_sortKeys;
	
	QSet<QString> m_ids;
	QList<QVariant> m_data;
};

#endif
