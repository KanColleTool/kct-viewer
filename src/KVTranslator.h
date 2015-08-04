#ifndef KVTRANSLATOR_H
#define KVTRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVariant>
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonValue>

class KVTranslator : public QObject
{
	friend class KCTViewerTest;

	Q_OBJECT

public:
	static KVTranslator& instance();
	
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
	void report(const QString &line, const QString &lastPathComponent, const QString &key);

signals:
	void loadFinished();
	void loadFailed(QString error);
	void waitingForLoad();

private slots:
	void translationRequestFinished();
	void blacklistRequestFinished();

protected:
	enum { created, loading, loaded, failed } state, blacklistState;
	QFile cacheFile;
	QNetworkAccessManager manager;

	struct ReportQueueEntry { QString line, lastPathComponent, key; };
	QList<ReportQueueEntry> reportQueue;

private:
	// Singleton stuff
	KVTranslator(QObject *parent = 0);
	KVTranslator(const KVTranslator&);
	KVTranslator& operator=(const KVTranslator&);
	virtual ~KVTranslator();
};

#endif
