#include "KVNetworkAccessManager.h"

#include <QDebug>

#include "KVNetworkReply.h"

KVNetworkAccessManager::KVNetworkAccessManager(QObject *parent) :
	QNetworkAccessManager(parent),
	trackedGETProgress(0), trackedGETTotalSize(0)
{

}

QNetworkReply* KVNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData)
{
	QNetworkRequest request = req;
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1667.0 Safari/537.36");

	if(request.url().host() != "localhost" && request.url().host() != "127.0.0.1") {
		if(op == QNetworkAccessManager::PostOperation) {
			//qDebug() << "POST" << request.url().path();
			QNetworkReply *r = QNetworkAccessManager::createRequest(op, request, outgoingData);
			KVNetworkReply *reply = new KVNetworkReply(r->parent(), r, this, translation);
			return reply;
		}
	}

	QNetworkReply *reply = QNetworkAccessManager::createRequest(op, request, outgoingData);

	// If the request if for an SWF or MP3 file, track it and report progress
	if(req.url().path().endsWith(".swf") || req.url().path().endsWith(".mp3"))
	{
		connect(reply, SIGNAL(metaDataChanged()), this, SLOT(trackedGETMetaDataChanged()));
		connect(reply, SIGNAL(readyRead()), this, SLOT(trackedGETReadyRead()));
		connect(reply, SIGNAL(finished()), this, SLOT(trackedGETFinished()));
	}

	return reply;
}

void KVNetworkAccessManager::trackedGETMetaDataChanged()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

	// For replies with enough headers to fire metaDataChanged() multiple times:
	// Make sure not to append the length more than once
	if(!reply->property("lengthAlreadyAdded").toBool())
	{
		// Only add the total size if it's present and valid
		bool ok;
		qint64 length = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong(&ok);
		if(ok)
		{
			trackedGETTotalSize += length;
			reply->setProperty("lengthAlreadyAdded", true);
		}
	}
}

void KVNetworkAccessManager::trackedGETReadyRead()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
	bool ok;
	/*qint64 length = */reply->header(QNetworkRequest::ContentLengthHeader).toLongLong(&ok);

	// Don't report progress for replies without a Content-Length
	if(ok)
	{
		trackedGETProgress += reply->bytesAvailable();
		emit trackedProgressChanged(trackedGETProgress, trackedGETTotalSize);
	}
}

void KVNetworkAccessManager::trackedGETFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
	bool ok;
	qint64 length = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong(&ok);

	if(ok)
	{
		trackedGETProgress -= length;
		if(trackedGETProgress < 0)
			trackedGETProgress = 0;

		trackedGETTotalSize -= length;
		if(trackedGETTotalSize < 0)
			trackedGETTotalSize = 0;

		emit trackedProgressChanged(trackedGETProgress, trackedGETTotalSize);
	}
}
