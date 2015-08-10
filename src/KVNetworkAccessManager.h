#ifndef KVNETWORKACCESSMANAGER_H
#define KVNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class KVNetworkReply;

/**
 * Intercepting network adapter.
 */
class KVNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
	
	/// Apply translation to the data in transit?
	Q_PROPERTY(bool useTranslation READ useTranslation WRITE setUseTranslation NOTIFY useTranslationChanged);
	
	/// User agent to apply to outgoing requests
	Q_PROPERTY(QString userAgent READ userAgent WRITE setUserAgent NOTIFY userAgentChanged);
	
public:
	/**
	 * Constructor.
	 * @param  parent Parent object
	 */
	explicit KVNetworkAccessManager(QObject *parent = 0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVNetworkAccessManager();
	
	
	
	/**
	 * Should the given request be intercepted?
	 * 
	 * @param  op   Method
	 * @param  req  Request descriptor
	 * @param  body Request body
	 */
	bool shouldIntercept(Operation op, const QNetworkRequest &req, QIODevice *body) const;
	
	
	
	bool useTranslation() const;				///< Gets the useTranslation property
	void setUseTranslation(bool v);				///< Sets the useTranslation property
	
	QString userAgent() const;					///< Gets the userAgent property
	void setUserAgent(const QString &v);		///< Sets the userAgent property
	
signals:
	void useTranslationChanged(bool v);			///< Emitted when useTranslation changes
	void userAgentChanged(const QString &v);	///< Emitted when userAgent changes
	
	/**
	 * Forwarded from KVNetworkReply::readyToPostProcess().
	 * 
	 * Called right before a request finishes, to give intercepting systems a
	 * chance to modify the returned data before it reaches its destination.
	 * 
	 * @param reply The reply to be processed
	 */
	void readyToPostProcess(KVNetworkReply *reply);
	
private slots:
	void interceptedRequestReadyToPostProcess();
	
protected:
	/**
	 * Creates a request.
	 * 
	 * Overridden to attach some extra handlers to certain requests.
	 * 
	 * @param  op   Method
	 * @param  req  Request descriptor
	 * @param  body Request body
	 * @return      A reply handle
	 */
	QNetworkReply* createRequest(Operation op, const QNetworkRequest &req, QIODevice *body);
	
private:
	bool m_useTranslation = false;
	QString m_userAgent = "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
};

#endif
