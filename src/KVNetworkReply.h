#ifndef KVNETWORKREPLY_H
#define KVNETWORKREPLY_H

#include <QNetworkReply>

class QSslConfiguration;
class KVNetworkAccessManager;

/**
 * Modified network reply.
 */
class KVNetworkReply : public QNetworkReply
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 * 
	 * @param parent    Parent object
	 * @param toCopy    Another reply to copy from
	 * @param mgr       Parent network access manager
	 * @param translate Translate this request in-transit?
	 */
	KVNetworkReply(QObject *parent, QNetworkReply *toCopy, QNetworkAccessManager *mgr, bool translate);
	
	/**
	 * Destructor.
	 * 
	 * \todo Shouldn't this be virtual?
	 */
	~KVNetworkReply();

	/**
	 * Copies attributes from another reply.
	 */
	void copyAttribute(QNetworkRequest::Attribute attr);

	/**
	 * Returns the number of bytes available.
	 */
	qint64 bytesAvailable() const;

	/// Forwarded to the copy
	void ignoreSslErrors(const QList<QSslError> &errors);
	/// Forwarded to the copy
	QSslConfiguration sslConfiguration() const;
	/// Forwarded to the copy
	void setSslConfiguration(const QSslConfiguration &config);
	/// Forwarded to the copy
	QNetworkAccessManager* manager() const;
	/// Forwarded to the copy
	bool event(QEvent *e);

	/// Forwarded to the copy
	void abort();
	/// Forwarded to the copy
	void ignoreSslErrors();
	
	/**
	 * Always returns true.
	 */
	bool isSequential() const;

protected slots:
	/**
	 * Called when the copy finishes loading.
	 */
	void handleResponse();
	
	/**
	 * Forwards data to a local KanColleTool instance.
	 * 
	 * @param body Response body
	 */
	void postToTool(const QByteArray &body);
	
	/**
	 * Writes the data to the local disk store.
	 * 
	 * @param body Response body
	 */
	void writeToDisk(const QByteArray &body);

protected:
	/// Forwarded to the copy
	qint64 readData(char *data, qint64 maxSize);

private:
	struct KVNetworkReplyPrivate *d;
};

#endif // KVNETWORKREPLY_H
