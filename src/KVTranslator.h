#ifndef KVTRANSLATOR_H
#define KVTRANSLATOR_H

#include <QObject>
#include <QString>
#include <QHash>

class KVTranslator : public QObject
{
	Q_OBJECT
	
public:
	/**
	 * Returns the shared instance.
	 */
	inline static KVTranslator& instance() {
		static KVTranslator _instance;
		return _instance;
	}
	
	/**
	 * Constructor.
	 * 
	 * @param parent Parent object
	 */
	KVTranslator(QObject *parent = 0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVTranslator();
	
public slots:
	/**
	 * Adds the given translation to the dictionary.
	 * 
	 * @param from Japanese phrase
	 * @param to   Translated phrase
	 */
	void addTranslation(const QString &from, const QString &to);
	
	/**
	 * Adds the given translation to the dictionary.
	 * 
	 * @param from CRC32 hash of the Japanese phrase
	 * @param to   Translated phrase
	 */
	void addTranslation(quint32 from, const QString &to);
	
public:
	/**
	 * Translates a Japanese phrase into the loaded language.
	 * 
	 * Returns the original string if no translation is loaded, if there's no
	 * match for the string's CRC32 sum, or if the result is a NULL variant.
	 */
	QString translate(const QString &phrase) const;
	
	/**
	 * Is the given phrase even translatable?
	 * 
	 * Examples of untranslatable strings are empty and numeric ones. This is
	 * checked by translate() before it attempts a translation lookup.
	 */
	bool isTranslatable(const QString &phrase) const;
	
protected:
	QHash<quint32, QString> dictionary;
};

#endif
