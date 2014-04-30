#ifndef KVNETWORKACCESSMANAGER_H
#define KVNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class QNetworkReply;
class KVNetworkAccessManager : public QNetworkAccessManager {
	Q_OBJECT

public:
	explicit KVNetworkAccessManager(QObject *parent = 0);
	bool translation;

signals:
	void trackedProgressChanged(qint64 progress, qint64 total);

protected slots:
	void trackedGETMetaDataChanged();
	void trackedGETReadyRead();
	void trackedGETFinished();

protected:
	QNetworkReply *createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData);
	qint64 trackedGETProgress, trackedGETTotalSize;
};

#endif // KVNETWORKACCESSMANAGER_H
