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
	
	/// Wrapped reply doing the actual work
	Q_PROPERTY(QNetworkReply *wrappedReply READ wrappedReply);
	
	/// Response body, setting it also resets the cursor to the start of it
	Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged);
	
public:
	/**
	 * Constructor.
	 * 
	 * @param manager      Associated network manager
	 * @param wrappedReply Wrapped reply that does all the work
	 * @param parent       Parent object
	 */
	KVNetworkReply(KVNetworkAccessManager *manager, QNetworkReply *wrappedReply, QObject *parent = 0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVNetworkReply();
	
	/**
	 * Syncs headers, attributes and properties to the wrapped reply's.
	 * 
	 * You normally do not have to call this yourself, unless you're doing
	 * someting funky with the wrapped reply.
	 */
	void syncToWrapped();
	
	QNetworkReply *wrappedReply() const;		///< Getter for wrappedReply
	
	const QByteArray data() const;				///< Getter for data
	void setData(const QByteArray &v);			///< Setter for data
	
protected:
	/**
	 * Syncs the Content-Length header to the data buffer's size.
	 */
	void syncSize();
	
signals:
	void dataChanged(QByteArray v);				///< Emitted when data changes
	
	/**
	 * Emitted right before finished().
	 * 
	 * If you want to postprocess the returned data in some way, this is your
	 * chance to do so.
	 */
	void readyToPostProcess();
	
private slots:
	void wrappedReplyDownloadProgress(qint64 received, qint64 total);
	void wrappedReplyUploadProgress(qint64 sent, qint64 total);
	void wrappedReplyError(QNetworkReply::NetworkError code);
	void wrappedReplyFinished();
	void wrappedReplyEncrypted();
	void wrappedReplyMetaDataChanged();
	void wrappedReplyPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *auth);
	void wrappedReplySslErrors(const QList<QSslError> &errors);
	
public:
	/**
	 * Sets the wrapped reply's read buffer size.
	 */
	virtual void setReadBufferSize(qint64 size) override;
	
	/**
	 * Opens the device and the wrapped reply.
	 * 
	 * You normally do not have to call this yourself.
	 * 
	 * @param  flags Mode flags
	 * @return       Did it succeed?
	 */
	virtual bool open(OpenMode flags) override;
	
	/**
	 * Closes the device and the wrapped reply.
	 */
	virtual void close() override;
	
	/**
	 * Returns the number of bytes available for reading.
	 */
	virtual qint64 bytesAvailable() const override;
	
	/**
	 * Replies are always sequential, put here for clarity.
	 */
	virtual bool isSequential() const override { return true; }
	
public slots:
	/**
	 * Aborts the request on the spot.
	 */
	virtual void abort() override;
	
	/**
	 * Ignore the specified SSL errors.
	 */
	virtual void ignoreSslErrors() override;
	
protected:
	/**
	 * Reads a chunk of data from the reply.
	 * 
	 * @param  data Pointer to where the data should be
	 * @param  len  Size of the destination buffer
	 * @return      The number of bytes actually read
	 */
	virtual qint64 readData(char *data, qint64 len) override;
	
private:
	QByteArray m_data;
	qint64 m_dataOffset = 0;
	
	QNetworkReply *m_wrappedReply;
};

#endif
