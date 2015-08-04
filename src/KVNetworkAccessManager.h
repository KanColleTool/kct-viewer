#ifndef KVNETWORKACCESSMANAGER_H
#define KVNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class QNetworkReply;

/**
 * Modified network access manager.
 * 
 * This comes with the ability to translate strings in transit, track download
 * progress of large resources, and otherwise intercept network replies.
 */
class KVNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 * 
	 * @param  parent Parent object
	 */
	explicit KVNetworkAccessManager(QObject *parent = 0);
	
	bool translation;		///< Apply translation?
	bool cookieHack;		///< Apply cookie hack?

signals:
	/**
	 * Progress of tracked resources changed.
	 * 
	 * @param progress Bytes downloaded
	 * @param total    Total bytes
	 */
	void trackedProgressChanged(qint64 progress, qint64 total);

protected slots:
	/**
	 * Metadata for a tracked download changed.
	 */
	void trackedGETMetaDataChanged();
	
	/**
	 * Data for a tracked download is ready to read.
	 */
	void trackedGETReadyRead();
	
	/**
	 * A tracked download finished loading.
	 */
	void trackedGETFinished();

protected:
	/**
	 * Creates a request.
	 * 
	 * Overridden to attach some extra handlers to certain requests.
	 * 
	 * @param  op           Method
	 * @param  req          Request descriptor
	 * @param  outgoingData Request body
	 * @return              A reply handle
	 */
	QNetworkReply* createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData);
	
	
	
	qint64 trackedGETTotalSize;		///< Total size of all tracked downloads
	qint64 trackedGETProgress;		///< Tracked download progress
};

#endif // KVNETWORKACCESSMANAGER_H
