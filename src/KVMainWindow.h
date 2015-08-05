#ifndef KVMAINWINDOW_H
#define KVMAINWINDOW_H

#include "KVWebView.h"
#include <QMainWindow>

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
	
	
	
	QString server;		///< The user's server
	QString apiToken;	///< The user's API token
	
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
	
public:
	/**
	 * Web view hosting the game.
	 * 
	 * @see KVWebView
	 */
	KVWebView *webView;
	
private:
	Ui::KVMainWindow *ui;
};

#endif
