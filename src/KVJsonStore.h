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
	
	/// A list of all inserted IDs
	Q_PROPERTY(const QSet<QString>& ids READ ids NOTIFY idsChanged);
	
public:
	/**
	 * Constructor.
	 */
	KVJsonStore(QString idKey = "");
	
	/**
	 * Destructor.
	 */
	virtual ~KVJsonStore();
	
	QString idKey() const;						///< Getter for idKey
	void setIdKey(QString v);					///< Setter for idKey
	
	const QSet<QString>& ids() const;			///< Getter for ids
	const QList<QVariant>& data() const;		///< Getter for data
	
	/**
	 * Inserts a new object into the store.
	 */
	void add(const QVariant &v);
	
signals:
	void idKeyChanged(QString v);				///< Emitted when idKey changes
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
	
private:
	QString m_idKey;
	QList<QString> m_sortKeys;
	
	QSet<QString> m_ids;
	QList<QVariant> m_data;
};

#endif
