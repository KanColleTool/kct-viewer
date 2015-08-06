#ifndef KVGAMEWRAPPER_H
#define KVGAMEWRAPPER_H

#include <QObject>
#include <QWebPage>

class KVGameWrapper : public QObject
{
	Q_OBJECT
	
	/// The user's server
	Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
	/// The user's API token
	Q_PROPERTY(QString apiToken READ apiToken WRITE setApiToken NOTIFY apiTokenChanged)
	
	/// The web page hosting the game
	Q_PROPERTY(QWebPage* page READ page);
	
public:
	/**
	 * Constructor.
	 * 
	 * @param parent Parent object
	 */
	KVGameWrapper(QObject *parent = 0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVGameWrapper();
	
	
	
	/**
	 * Sets (decodes) the API Link.
	 * 
	 * Note that the original string isn't saved; it's decoded into its server
	 * and API key parts.
	 */
	void setApiLink(const QUrl &url);
	QUrl apiLink() const;				///< API link for current credentials
	
	QString server() const;				///< Gets the server property
	void setServer(const QString &v);	///< Sets the server property
	
	QString apiToken() const;			///< Gets the apiToken property
	void setApiToken(const QString &v);	///< Sets the apiToken property
	
	QWebPage *page() const;				///< Getter for the page property
	
signals:
	void serverChanged();				///< Emitted when server changes
	void apiTokenChanged();				///< Emitted when apiToken changes
	
public slots:
	/**
	 * Loads the game and attempts to initialize it.
	 * 
	 * This requires a correct set of credentials to work properly.
	 */
	void startGame();
	
private slots:
	void onPageLoadFinished(bool ok);
	
private:
	QString m_server;
	QString m_apiToken;
	
	QWebPage *m_page;
};

#endif
