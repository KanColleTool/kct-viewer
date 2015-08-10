#include "KVNetworkReply.h"

KVNetworkReply::KVNetworkReply(KVNetworkAccessManager *manager, QNetworkReply *wrappedReply, QObject *parent):
	QNetworkReply(parent),
	m_wrappedReply(wrappedReply)
{
	this->open(QIODevice::ReadOnly);
	
	connect(m_wrappedReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(wrappedReplyDownloadProgress(qint64, qint64)));
	connect(m_wrappedReply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(wrappedReplyUploadProgress(qint64, qint64)));
	connect(m_wrappedReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(wrappedReplyError(QNetworkReply::NetworkError)));
	connect(m_wrappedReply, SIGNAL(finished()), this, SLOT(wrappedReplyFinished()));
	connect(m_wrappedReply, SIGNAL(encrypted()), this, SLOT(wrappedReplyEncrypted()));
	connect(m_wrappedReply, SIGNAL(metaDataChanged()), this, SLOT(wrappedReplyMetaDataChanged()));
	connect(m_wrappedReply, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)), this, SLOT(wrappedReplyPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)));
	connect(m_wrappedReply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(wrappedReplySslErrors(const QList<QSslError>&)));
}

KVNetworkReply::~KVNetworkReply()
{
	
}

void KVNetworkReply::syncToWrapped()
{
	static const QList<QNetworkRequest::Attribute> syncedAttrs {
		QNetworkRequest::HttpStatusCodeAttribute,
		QNetworkRequest::HttpReasonPhraseAttribute,
		QNetworkRequest::RedirectionTargetAttribute,
		QNetworkRequest::ConnectionEncryptedAttribute,
		QNetworkRequest::SourceIsFromCacheAttribute,
		QNetworkRequest::HttpPipeliningWasUsedAttribute,
		QNetworkRequest::SpdyWasUsedAttribute,
	};
	
	this->setOperation(m_wrappedReply->operation());
	this->setUrl(m_wrappedReply->url());
	this->setRequest(m_wrappedReply->request());
	
	this->setError(m_wrappedReply->error(), m_wrappedReply->errorString());
	this->setFinished(m_wrappedReply->isFinished());
	
	for (QNetworkRequest::Attribute attr : syncedAttrs) {
		this->setAttribute(attr, m_wrappedReply->attribute(attr));
	}
	
	for (QNetworkReply::RawHeaderPair pair : m_wrappedReply->rawHeaderPairs()) {
		this->setRawHeader(pair.first, pair.second);
	}
}



QNetworkReply* KVNetworkReply::wrappedReply() const { return m_wrappedReply; }

const QByteArray KVNetworkReply::data() const { return m_data; }
void KVNetworkReply::setData(const QByteArray &v)
{
	m_data = v;
	m_dataOffset = 0;
	this->syncSize();
	emit dataChanged(v);
}



void KVNetworkReply::syncSize()
{
	this->setHeader(QNetworkRequest::ContentLengthHeader, m_data.size());
}



void KVNetworkReply::wrappedReplyDownloadProgress(qint64 received, qint64 total)
{
	emit downloadProgress(received, total);
}

void KVNetworkReply::wrappedReplyUploadProgress(qint64 sent, qint64 total)
{
	emit uploadProgress(sent, total);
}

void KVNetworkReply::wrappedReplyError(QNetworkReply::NetworkError code)
{
	this->open(QIODevice::ReadOnly);
	this->syncToWrapped();
	
	emit error(code);
}

void KVNetworkReply::wrappedReplyFinished()
{
	this->setData(m_wrappedReply->readAll());
	this->syncToWrapped();
	
	emit readyToPostProcess();
	emit finished();
}

void KVNetworkReply::wrappedReplyEncrypted()
{
	emit encrypted();
}

void KVNetworkReply::wrappedReplyMetaDataChanged()
{
	this->syncToWrapped();
	emit metaDataChanged();
}

void KVNetworkReply::wrappedReplyPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *auth)
{
	emit preSharedKeyAuthenticationRequired(auth);
}

void KVNetworkReply::wrappedReplySslErrors(const QList<QSslError> &errors)
{
	emit sslErrors(errors);
}



void KVNetworkReply::setReadBufferSize(qint64 size)
{
	m_wrappedReply->setReadBufferSize(size);
	QNetworkReply::setReadBufferSize(size);
}

bool KVNetworkReply::open(OpenMode flags)
{
	return m_wrappedReply->open(flags) && QIODevice::open(flags);
}

void KVNetworkReply::close()
{
	m_wrappedReply->close();
	QNetworkReply::close();
}

qint64 KVNetworkReply::bytesAvailable() const
{
	return (m_data.size() - m_dataOffset) + QIODevice::bytesAvailable();
}



void KVNetworkReply::abort()
{
	m_wrappedReply->abort();
}

void KVNetworkReply::ignoreSslErrors()
{
	m_wrappedReply->ignoreSslErrors();
	QNetworkReply::ignoreSslErrors();
}

qint64 KVNetworkReply::readData(char *data, qint64 len)
{
	len = qMin(len, m_data.size() - m_dataOffset);
	memcpy(data, m_data.constData() + m_dataOffset, len);
	m_dataOffset += len;
	
	return len;
}
