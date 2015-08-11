#ifndef KVTRANSLATOR_H
#define KVTRANSLATOR_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QJsonDocument>

/**
 * Translator for in-game strings.
 */
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
	 * match for the string's CRC32 sum, or if the string is untranslatable.
	 * 
	 * @see isTranslatable()
	 */
	QString translate(const QString &phrase) const;
	
	/**
	 * Translates a JSON document from Japanese.
	 * 
	 * This will walk through every string in the document (excluding keys) and
	 * attempt to translate it.
	 * 
	 * @param  doc A JSON document
	 * @return     A translated JSON document
	 */
	QJsonDocument translate(const QJsonDocument &doc) const;
	
	/**
	 * Is the given phrase even translatable?
	 * 
	 * Examples of untranslatable strings are empty and numeric ones.
	 */
	bool isTranslatable(const QString &phrase) const;
	
protected:
	/**
	 * Recursively walks through a JSON object.
	 * 
	 * This will recursively call itself until it has travelled down every
	 * possible path of the document.
	 * 
	 * @param key The key the item was found at
	 * @param obj The current object
	 */
	void walk(QJsonValueRef value, const QString &key = "") const;
	
	/**
	 * Dictionary of registered translations.
	 * 
	 * Keys are CRC32 checksums of the original strings, values are translated
	 * counterparts in the current language. The translator does not know or
	 * care what it's translating to, as long as it's representable in UTF-8.
	 */
	QHash<quint32, QString> dictionary;
};

#endif
