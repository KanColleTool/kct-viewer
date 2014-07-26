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

class KVMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	KVMainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

protected slots:
	void postConstructorSetup();

protected:
	void loadTranslation(QString language="en");
	void loadSettings(bool start=false);
	void generateAPILinkURL();

public slots:
	void loadBundledIndex();
	void checkForUpdates();
	void askForAPILink(bool reload=true);
	void openSettings();
	void implementSettings(bool start=false);
	void clearCache();
	void showAbout();
    void screenshot();

private slots:
	void onVersionCheckFinished();
	void onLoadStarted();
	void onLoadFinished(bool ok);
	void onWaitingForTranslation();
	void onTranslationLoaded();
	void onTranslationLoadFailed(QString error);
	void onTrackedProgressChanged(qint64 progress, qint64 total);

	void setHTMLAPILink();

	// Never actually emmitted yet; I'm trying to get the proxy
	// stable before I start poking at the responses.
	//void onAPIError(KVProxyServer::APIStatus error);

protected:
	KVWebView *webView;

	KVNetworkAccessManager *wvManager, manager;
	QNetworkDiskCache *cache;
	QMessageBox *translationMsgBox;

	QString server, apiToken;
	QUrl apiLink;

	bool showTaskbarProgress;

#ifdef Q_OS_WIN
	QWinTaskbarButton *taskbarButton;
#endif

private:
	Ui::KVMainWindow *ui;
};

#endif
