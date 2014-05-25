#ifndef KVTRANSLATOR_H
#define KVTRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QJsonValue>

class KVTranslator : public QObject
{
	friend class KCTViewerTest;
	
	Q_OBJECT

public:
	static KVTranslator* instance();
	bool isLoaded();
	
	bool reportUntranslated;
	QVariantMap translation, reportBlacklist;

public slots:
	void loadTranslation(QString language = "en");

	QString translate(const QString &line, QString lastPathComponent = "", QString key = "");
	QByteArray translateJson(QByteArray json, QString lastPathComponent = "");
	QString fixTime(const QString &time);

protected:
	bool parseTranslationData(const QByteArray &data);
	QJsonValue _walk(QJsonValue value, QString lastPathComponent, QString key="");
	static QString jsonEscape(const QString &str);

signals:
	void loadFinished();
	void loadFailed(QString error);
	void waitingForLoad();

private slots:
	void translationRequestFinished();

protected:
	enum JsonContext {
		Start, End,
		Object, Array,
		Key, AfterKey,
		Value, NonString, String, AfterValue,
		Invalid
	};

	struct JsonState {
		JsonContext context;
		int start, arri;
		JsonState();
		JsonState(JsonContext c, int s, int ai=-1);
		operator QString() const;
	};

	struct Reader {
		int i;
		const QByteArray &data;
		Reader(const QByteArray &json);

		const QByteArray readTo(int e);
		const QByteArray readAll();
	};

	enum { created, loading, loaded, failed } state;
	QFile cacheFile;
	QNetworkAccessManager manager;

	// Singleton stuff
	static KVTranslator *m_instance;

	KVTranslator(QObject *parent = 0);
	KVTranslator(const KVTranslator&);
	virtual ~KVTranslator();

	KVTranslator& operator=(const KVTranslator&);
};

#endif
