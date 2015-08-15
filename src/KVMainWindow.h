#ifndef KVMAINWINDOW_H
#define KVMAINWINDOW_H

#include "KVWebView.h"
#include "KVGameWrapper.h"
#include "KVJsonStore.h"
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
	
protected:
	/**
	 * Sets things up.
	 */
	void setup();
	
	/**
	 * Connect signals.
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
	
private:
private slots:
	void on_actionEnterAPILink_triggered();
	void on_actionSettings_triggered();
	void on_actionClearCache_triggered();
	void on_actionReset_triggered();
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionScreenshot_triggered();
	void on_actionGetAPILink_triggered();
	
	void onMissingTranslation(const QString &phrase, const QString &key, const QUrl &source);
	
protected:
	/**
	 * Web view hosting the game.
	 * 
	 * @see KVWebView
	 */
	KVWebView *webView;
	
	/**
	 * Wrapper for the game's state.
	 */
	KVGameWrapper *game;
	
	/**
	 * Store for untranslated lines.
	 */
	KVJsonStore untranslated;
	
private:
	Ui::KVMainWindow *ui;
};

#endif
