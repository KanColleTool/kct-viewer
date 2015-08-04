#ifndef KVMAINWINDOW_H
#define KVMAINWINDOW_H

#include "KVWebView.h"
#include "KVNetworkAccessManager.h"

#include <QMainWindow>
#include <QWebView>
#include <QMessageBox>
#include <QNetworkDiskCache>

#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif

namespace Ui {
	class KVMainWindow;
}

/**
 * Main application window.
 */
class KVMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * Designated constructor.
	 */
	KVMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

protected slots:
	/**
	 * Perform setup tasks that cannot be done within the constructor.
	 * 
	 * This is called on the very first timer tick within the application's run
	 * loop, which means it will have access to things like a native window
	 * handle.
	 */
	void postConstructorSetup();

protected:
	/**
	 * Starts loading a translation file.
	 * 
	 * @param language The language code to load
	 */
	void loadTranslation(QString language = "en");
	
	/**
	 * Loads stored user settings.
	 * 
	 * Also loads the game, unless isStarting is false.
	 * 
	 * \todo Stop that, that's dumb.
	 * 
	 * @param isStarting Is the application starting?
	 */
	void loadSettings(bool isStarting = false);
	
	/**
	 * Generates an API link.
	 * 
	 * This uses KVMainWindow::server and KVMainWindow::apiToken, and populates
	 * KVMainWindow::apiLink.
	 * 
	 * \todo Just make it take these two as parameters and return the URL.
	 */
	void generateAPILinkURL();

public slots:
	/**
	 * Loads the bundled index.html file into the web view.
	 * 
	 * @see onLoadFinished
	 */
	void loadBundledIndex();
	
	/**
	 * Checks for updates to the application.
	 * 
	 * @see onVersionCheckFinished
	 */
	void checkForUpdates();
	
	/**
	 * Prompts the user for an API link.
	 * 
	 * @param reload Reload the game afterwards?
	 */
	void askForAPILink(bool reload = true);
	
	/**
	 * Opens the settings dialogue.
	 */
	void openSettings();
	
	/**
	 * Applies new settings from the settings dialogue.
	 * 
	 * \todo Remove isStarting
	 * \todo Rename this to something better
	 * 
	 * @param isStarting Is the application starting?
	 */
	void implementSettings(bool isStarting = false);
	
	/**
	 * Clears the cache and reloads the game.
	 * 
	 * TOOD: Stop it with the side effects, seriously.
	 */
	void clearCache();
	
	/**
	 * Opens the "About KCTViewer" dialog.
	 */
	void showAbout();
	
	/**
	 * Takes a screenshot.
	 * 
	 * \todo Rename this to something better.
	 */
	void screenshot();
	
	/**
	 * Use the web view to fetch an API link.
	 * 
	 * \todo Rename this to something better.
	 */
	void fetchAPILink();

private slots:
	/**
	 * Called when a version check finishes.
	 * 
	 * @see checkForUpdates
	 */
	void onVersionCheckFinished();
	
	/**
	 * Called when the game starts loading.
	 */
	void onLoadStarted();
	
	/**
	 * Called when the game finishes loading.
	 * 
	 * @param ok Did the loading succeed?
	 */
	void onLoadFinished(bool ok);
	
	/**
	 * Called when the translation starts loading.
	 * 
	 * \todo Rename this.
	 */
	void onWaitingForTranslation();
	
	/**
	 * Called when the translation finishes loading.
	 */
	void onTranslationLoaded();
	
	/**
	 * Called when the translation fails to load.
	 * @param error An error message
	 */
	void onTranslationLoadFailed(QString error);
	
	/**
	 * Called when the loading progress of a tracked file changes.
	 * 
	 * A tracked file is anything we identify as a significant resource, based
	 * on its URL (see KVNetworkAccessManager). These progress updates are
	 * used to update any relevant progress bars.
	 * 
	 * @param progress Bytes fetched
	 * @param total    Total bytes
	 */
	void onTrackedProgressChanged(qint64 progress, qint64 total);
	
	/**
	 * Called when the game frame finishes loading, when extracting an API link.
	 * 
	 * \todo Rename this.
	 * 
	 * @param url The URL of the game embed
	 */
	void onGameFrameFinished(QUrl url);

	/**
	 * Updates the API link on the HTML embed element.
	 * 
	 * \todo Give this a sane function signature.
	 */
	void setHTMLAPILink();

	// Never actually emmitted yet; I'm trying to get the proxy
	// stable before I start poking at the responses.
	//void onAPIError(KVProxyServer::APIStatus error);
	
private slots:
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
	
	/**
	 * Message box for translation load status.
	 * 
	 * \todo Rename this.
	 */
	QMessageBox *translationMsgBox;
	
	/**
	 * Network access manager for the game.
	 */
	KVNetworkAccessManager *wvManager;
	
	/**
	 * Cache for static files.
	 */
	QNetworkDiskCache *cache;
	
	/**
	 * Network access manager for non-game related operations.
	 */
	QNetworkAccessManager manager;



	QString server;					///< The user's server
	QString apiToken;				///< The user's API token
	QUrl apiLink;					///< Full API link

	bool showTaskbarProgress;		///< Show progress in the W7+ taskbar?

#ifdef Q_OS_WIN
	QWinTaskbarButton *taskbarButton;	///< Taskbar button handle
#endif

private:
	Ui::KVMainWindow *ui;
};

#endif
