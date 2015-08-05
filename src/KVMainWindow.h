#ifndef KVMAINWINDOW_H
#define KVMAINWINDOW_H

#include "KVWebView.h"
#include <QMainWindow>
#include <QUrl>

namespace Ui {
	class KVMainWindow;
}

/**
 * Main application window.
 */
class KVMainWindow : public QMainWindow
{
	Q_OBJECT
	
	/// The user's server
	Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
	/// The user's API token
	Q_PROPERTY(QString apiToken READ apiToken WRITE setApiToken NOTIFY apiTokenChanged)
	
public:
	/**
	 * Constructor.
	 * 
	 * @param parent Parent window
	 * @param flags Window flags
	 */
	KVMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVMainWindow();
	
	
	
	/**
	 * Sets (decodes) the API Link.
	 * 
	 * Note that the original string isn't saved; it's decoded into its server
	 * and API key parts.
	 */
	void setApiLink(const QUrl &url);
	
	/**
	 * Generate an API link from current credentials.
	 */
	QUrl apiLink() const;
	
	QString server() const;				///< Gets the server property
	void setServer(const QString &v);	///< Sets the server property
	
	QString apiToken() const;			///< Gets the apiToken property
	void setApiToken(const QString &v);	///< Sets the apiToken property
	
signals:
	void serverChanged();				///< Emitted when server changes
	void apiTokenChanged();				///< Emitted when apiToken changes
	
protected:
	/**
	 * Sets things up.
	 */
	void setup();
	
	/**
	 * Connects signals.
	 */
	void connectSignals();
	
	/**
	 * Autoresizes and locks the window size.
	 */
	void autoLockWindowSize();
	
public:
public slots:
	/**
	 * Attempts to load stored credentials.
	 * 
	 * @return Were any found?
	 */
	bool loadCredentials();
	
	/**
	 * Saves credentials to the settings.
	 */
	void storeCredentials();
	
	/**
	 * Loads the game into the web view.
	 */
	void startGame();
	
private:
private slots:
	void onWebViewLoadFinished(bool success);
	
	void on_actionEnterAPILink_triggered();
	void on_actionSettings_triggered();
	void on_actionClearCache_triggered();
	void on_actionReset_triggered();
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionScreenshot_triggered();
	void on_actionGetAPILink_triggered();
	
protected:
	/**
	 * Web view hosting the game.
	 * 
	 * @see KVWebView
	 */
	KVWebView *webView;
	
private:
	Ui::KVMainWindow *ui;
	
	QString m_server;
	QString m_apiToken;
};

#endif
