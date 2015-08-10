#ifndef KVNETWORKREPLY_H
#define KVNETWORKREPLY_H

#include <QNetworkReply>
#include "KVNetworkAccessManager.h"

/**
 * Intercepting wrapper around a QNetworkReply.
 */
class KVNetworkReply : public QNetworkReply
{
	Q_OBJECT
	
	Q_PROPERTY(QNetworkReply *wrappedReply READ wrappedReply);
	Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged);
	
public:
	KVNetworkReply(KVNetworkAccessManager *manager, QNetworkReply *wrappedReply, QObject *parent = 0);
	virtual ~KVNetworkReply();
	
	void syncToWrapped();
	
	QNetworkReply *wrappedReply() const;
	
	QByteArray data() const;
	void setData(const QByteArray &v);
	
signals:
	void dataChanged(QByteArray v);
	
protected slots:
	void wrappedReplyDownloadProgress(qint64 received, qint64 total);
	void wrappedReplyUploadProgress(qint64 sent, qint64 total);
	void wrappedReplyError(QNetworkReply::NetworkError code);
	void wrappedReplyFinished();
	void wrappedReplyEncrypted();
	void wrappedReplyMetaDataChanged();
	void wrappedReplyPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *auth);
	void wrappedReplySslErrors(const QList<QSslError> &errors);
	
public:
	virtual void setReadBufferSize(qint64 size) override;
	
	virtual bool open(OpenMode flags) override;
	virtual void close() override;
	virtual qint64 bytesAvailable() const override;
	virtual bool isSequential() const override { return true; }
	
public slots:
	virtual void abort() override;
	virtual void ignoreSslErrors() override;
	
protected:
	virtual qint64 readData(char *data, qint64 len) override;
	
private:
	QByteArray m_data;
	qint64 m_dataOffset = 0;
	
	QNetworkReply *m_wrappedReply;
};

#endif
