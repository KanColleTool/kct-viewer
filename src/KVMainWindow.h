#ifndef KVMAINWINDOW_H
#define KVMAINWINDOW_H

#include "KVWebView.h"
#include <QMainWindow>
#include <QUrl>

namespace Ui {
	class KVMainWindow;
}

class KVMainWindow : public QMainWindow
{
	Q_OBJECT
	
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
	 * Generate an API link from current credentials.
	 */
	QUrl apiLink() const;
	
	/**
	 * Sets (decodes) the API Link.
	 * 
	 * Note that the original string isn't saved; it's decoded into its server
	 * and API key parts.
	 */
	void setApiLink(const QUrl &url);
	
	/**
	 * Sets the stored credentials.
	 */
	void setCredentials(const QString &server, const QString &apiToken);
	
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
	
protected:
protected slots:
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
	
	QString server;			///< The user's server
	QString apiToken;		///< The user's API token
	
private:
	Ui::KVMainWindow *ui;
};

#endif
