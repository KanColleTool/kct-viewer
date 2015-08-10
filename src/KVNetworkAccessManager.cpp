#include "KVNetworkAccessManager.h"
#include "KVNetworkReply.h"
#include <QNetworkReply>

KVNetworkAccessManager::KVNetworkAccessManager(QObject *parent):
	QNetworkAccessManager(parent)
{
	
}

KVNetworkAccessManager::~KVNetworkAccessManager()
{
	
}



bool KVNetworkAccessManager::shouldIntercept(Operation op, const QNetworkRequest &req, QIODevice *body) const
{
	if (op == PostOperation) {
		const QString &host = req.url().host();
		if(host != "localhost" && host != "127.0.0.1" && !host.contains(".dmm.com")) {
			return true;
		}
	}
	
	return false;
}



bool KVNetworkAccessManager::useTranslation() const { return m_useTranslation; }
void KVNetworkAccessManager::setUseTranslation(bool v) { m_useTranslation = v; emit useTranslationChanged(v); }

QString KVNetworkAccessManager::userAgent() const { return m_userAgent; }
void KVNetworkAccessManager::setUserAgent(const QString &v) { m_userAgent = v; emit userAgentChanged(v); }



void KVNetworkAccessManager::interceptedRequestReadyToPostProcess()
{
	emit readyToPostProcess(qobject_cast<KVNetworkReply*>(QObject::sender()));
}



QNetworkReply* KVNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req_, QIODevice *body)
{
	QNetworkRequest req(req_);
	req.setRawHeader("User-Agent", m_userAgent.toUtf8());
	
	QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, body);
	
	if(this->shouldIntercept(op, req, body)) {
		// reply = new KVNetworkReply(reply->parent(), reply, this, this->useTranslation());
		reply = new KVNetworkReply(this, reply, reply->parent());
		connect(reply, SIGNAL(readyToPostProcess()), this, SLOT(interceptedRequestReadyToPostProcess()));
	}
	
	return reply;
}
