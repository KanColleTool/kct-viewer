#ifndef KVTRANSLATOR_H
#define KVTRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVariant>
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonValue>

/**
 * Translator for in-game text.
 */
class KVTranslator : public QObject
{
	Q_OBJECT

public:
	/**
	 * Returns the shared instance.
	 */
	static KVTranslator& instance();
	
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
	
	
	
	/**
	 * Is the translation loaded?
	 */
	bool isLoaded();
	
	bool reportUntranslated;			///< Should untranslated lines be reported?
	QVariantMap translation;			///< Translation data
	QVariantMap reportBlacklist;		///< Blacklist of keys to NOT report

public slots:
	/**
	 * Starts loading the translation for the given language.
	 * 
	 * @param language Language code to load
	 */
	void loadTranslation(QString language = "en");

	/**
	 * Translates the given string into the loaded language.
	 * 
	 * @param  line              Line to be translated
	 * @param  lastPathComponent The last path component of the file's URL, for reporting
	 * @param  key               JSON key, if any
	 * @return                   A translated line, or the original line
	 */
	QString translate(const QString &line, QString lastPathComponent = "", QString key = "");
	
	/**
	 * Translates a JSON blob.
	 * 
	 * The given JSON may have an "svdata=" prefix, and it will be preserved
	 * if present.
	 * 
	 * @param  json              JSON string to translate
	 * @param  lastPathComponent The last path component of the file's URL, for reporting
	 * @return                   A JSON blob with translated values
	 */
	QByteArray translateJson(QByteArray json, QString lastPathComponent = "");
	
	/**
	 * Fixes a timestamp to be in JST.
	 * 
	 * \todo Rename this.
	 * 
	 * @param  time Timestamp to adjust
	 * @return      An adjusted timestamp
	 */
	QString fixTime(const QString &time);

protected:
	/**
	 * Parses a loaded JSON blob of translation data.
	 * 
	 * @return      Was it successfully parsed?
	 */
	bool parseTranslationData(const QByteArray &data);
	
	/**
	 * Recursively walks a JSON tree, translating it.
	 * 
	 * @param  value             The leaf to walk from
	 * @param  lastPathComponent The last path component of the file's URL, for reporting
	 * @param  key               The leaf's key
	 * @return                   A translated leaf
	 */
	QJsonValue _walk(QJsonValue value, QString lastPathComponent, QString key="");
	
	/**
	 * Reports an untranslated line to the server.
	 * 
	 * @param line              An untranslated line
	 * @param lastPathComponent The last path component of the file's URL
	 * @param key               JSON key for the line
	 */
	void report(const QString &line, const QString &lastPathComponent, const QString &key);

signals:
	/**
	 * Emitted when the translation finishes loading.
	 */
	void loadFinished();
	
	/**
	 * Emitted when the translation fails to load.
	 * @param error An error mesage
	 */
	void loadFailed(QString error);
	
	/**
	 * Emitted when the translation starts loading.
	 */
	void waitingForLoad();

private slots:
	/**
	 * Called when the translation finishes loading.
	 */
	void translationRequestFinished();
	
	/**
	 * Called when the blacklist finishes loading.
	 */
	void blacklistRequestFinished();

public:
	/**
	 * Loading state for a resouce.
	 * 
	 * \todo Capitalization.
	 */
	enum State {
		created,		///< Created
		loading,		///< Loading
		loaded,			///< Loaded
		failed,			///< Failed
	};
	
	State state;					///< Loading state for the translation
	State blacklistState;			///< Loading state for the blacklist
	
protected:
	QFile cacheFile;				///< Cache file for the translation
	QNetworkAccessManager manager;	///< Network access manager

	/**
	 * An entry in the report queue.
	 */
	struct ReportQueueEntry {
		QString line;				///< The untranslated line
		QString lastPathComponent;	///< The file's last path component
		QString key;				///< THe line's JSON key
	};
	
	/**
	 * Queued lines for submission.
	 * 
	 * This builds up if untranslated lines are found before the report
	 * blacklist has been loaded, and consumed by report().
	 */
	QList<ReportQueueEntry> reportQueue;
};

#endif
