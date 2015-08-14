#ifndef KVGAMEWRAPPER_H
#define KVGAMEWRAPPER_H

#include <QObject>
#include <QWebPage>

class KVNetworkReply;

/**
 * Wrapper for a game client.
 * 
 * This encapsulates all interactions and handling of a KanColle client,
 * decoupling it from the presenting UI.
 * 
 * To display it in a widget, add the wrapper's page() to a QWebView, and call
 * startGame() to initialize and start it the game.
 */
class KVGameWrapper : public QObject
{
	Q_OBJECT
	
	/// The user's server
	Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
	/// The user's API token
	Q_PROPERTY(QString apiToken READ apiToken WRITE setApiToken NOTIFY apiTokenChanged)
	
	/// The web page hosting the game
	Q_PROPERTY(QWebPage *page READ page);
	
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
	QUrl apiLink() const;					///< API link for current credentials
	
	QString server() const;					///< Gets the server property
	void setServer(const QString &v);		///< Sets the server property
	
	QString apiToken() const;				///< Gets the apiToken property
	void setApiToken(const QString &v);		///< Sets the apiToken property
	
	QWebPage *page() const;					///< Getter for the page property
	
signals:
	void serverChanged(const QString &v);	///< Emitted when server changes
	void apiTokenChanged(const QString &v);	///< Emitted when apiToken changes
	
public slots:
	/**
	 * Loads the game and attempts to initialize it.
	 * 
	 * This requires a correct set of credentials to work properly.
	 */
	void startGame();
	
private slots:
	/**
	 * Called when the page finishes loading.
	 * 
	 * @param ok Did it load successfully?
	 */
	void onPageLoadFinished(bool ok);
	
	/**
	 * Called when an intercepted reply is ready to postprocess.
	 * 
	 * @param reply An intercepted reply
	 */
	void onInterceptedRequestReadyToPostProcess(KVNetworkReply *reply);
	
private:
	QString m_server;
	QString m_apiToken;
	
	QWebPage *m_page;
};

#endif
