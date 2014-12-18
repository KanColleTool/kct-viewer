#include "KVMainWindow.h"
#include "ui_KVMainWindow.h"
#include "KVDefaults.h"
#include "KVSettingsDialog.h"
#include "KVNetworkAccessManager.h"
#include "KVTranslator.h"
#include "KVScreenshooter.h"

#include <QMenuBar>
#include <QMenu>
#include <QWebFrame>
#include <QWebPage>
#include <QInputDialog>
#include <QFile>
#include <QUrl>
#include <QUrlQuery>
#include <QApplication>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QHostAddress>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>
#include <QWebElement>

KVMainWindow::KVMainWindow(QWidget *parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	translationMsgBox(0),
	ui(new Ui::KVMainWindow)
{
	ui->setupUi(this);

	connect(ui->actionEnterAPILink, SIGNAL(triggered()), this, SLOT(askForAPILink()));
	connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));
	connect(ui->actionClearCache, SIGNAL(triggered()), this, SLOT(clearCache()));
	connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(loadBundledIndex()));
	connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(ui->actionScreenshot, SIGNAL(triggered()), this, SLOT(screenshot()));
	connect(ui->actionGetAPILink, SIGNAL(triggered()), this, SLOT(getAPILink()));

	// Set a custom network access manager to let us set up a cache and proxy.
	// Without a cache, the game takes ages to load.
	// Without a proxy, we can't do cool things like translating the game.
	wvManager = new KVNetworkAccessManager(this);
	connect(wvManager, SIGNAL(trackedProgressChanged(qint64,qint64)), this, SLOT(onTrackedProgressChanged(qint64,qint64)));

	// Set up a cache; a larger-than-normal disk cache is quite enough for our purposes
	cache = new QNetworkDiskCache(this);
	cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setMaximumCacheSize(1024*1024*1024); // Max 1GB cache storage
	wvManager->setCache(cache);

	// Can't set this up in the designer, since it doesn't like QWebView subclasses
	// Like, I literally can't promote QWebView widgets, go figure
	webView = new KVWebView(this);
	webView->setFixedSize(800, 480);
	this->centralWidget()->layout()->addWidget(webView);

	// Set up the web view, using our custom Network Access Manager
	webView->page()->setNetworkAccessManager(wvManager);

	// The context menu only contains "Reload" anyways
	webView->setContextMenuPolicy(Qt::PreventContextMenu);
	// These are so large that they create a need for themselves >_>
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

	connect(webView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
	connect(webView, &KVWebView::gameFrameFinished, this, &KVMainWindow::onGameFrameFinished);

	// Auto-adjust the window to fit its contents, and lock it to that size
	// As the web view is locked to 800x480, this will simply account for
	// differences in the menu bar size and such, and prevent resizing
	this->adjustSize();
	this->setFixedSize(this->width(), this->height());

	// Check for updates
	// Updates on Linux are handled by the package manager
#if !defined(Q_OS_LINUX)
	this->checkForUpdates();
#endif

	// Schedule a call to the post-constructor setup
	QTimer::singleShot(0, this, SLOT(postConstructorSetup()));

	this->loadSettings(true);
	this->loadBundledIndex();
}

void KVMainWindow::postConstructorSetup()
{
#ifdef Q_OS_WIN
	// This has to be done while we have a run loop, otherwise windowHandle() will return 0
	taskbarButton = new QWinTaskbarButton(this);
	taskbarButton->setWindow(this->windowHandle());
	taskbarButton->progress()->setRange(0, 10000);
#endif
}

void KVMainWindow::checkForUpdates()
{
	QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://kancolletool.github.io/VERSION")));
	connect(reply, SIGNAL(finished()), this, SLOT(onVersionCheckFinished()));
}

void KVMainWindow::loadTranslation(QString language)
{
	KVTranslator *translator = KVTranslator::instance();
	if(translator->isLoaded()) return;

	connect(translator, SIGNAL(waitingForLoad()), this, SLOT(onWaitingForTranslation()));
	connect(translator, SIGNAL(loadFinished()), this, SLOT(onTranslationLoaded()));
	connect(translator, SIGNAL(loadFailed(QString)), this, SLOT(onTranslationLoadFailed(QString)));
	translator->loadTranslation(language);
}

void KVMainWindow::loadBundledIndex()
{
	QFile file(":/index.html");
	if(file.open(QIODevice::ReadOnly))
	{
		webView->setHtml(file.readAll(), apiLink);
	}
	else
	{
		QMessageBox::critical(this, "Can't load resource", "Couldn't load the local resources needed to start the client.<br /><br />I have no idea how you even managed to make this happen, since the resources are supposed to be inside the executable, but it probably involved a recompilation that went wrong.<br /><br /><code>index.html</code> needs to be in the root of <code>resources.qrc</code>.");
		exit(1);
	}
}

void KVMainWindow::generateAPILinkURL()
{
	apiLink = QUrl(QString("http://%1/kcs/mainD2.swf?api_token=%2").arg(server, apiToken));
}

void KVMainWindow::askForAPILink(bool reload)
{
	// Get the link from the user
	QString link = QInputDialog::getText(this, "Enter API Link", "Please enter your API Link.<br /><br />It should look something like:<br /><code>http://125.6.XXX.XXX/kcs/mainD2.swf?api_token=xxxxxxxxxx...</code>");

	// If the link is empty, the user pressed cancel, for whatever reason
	if(link.isEmpty())
		return;

	// Make an URL and a Query from it
	QUrl url(link);
	QUrlQuery query(url);

	// Extract the important bits, and generate a well-formed URL from that
	// (It's important that nothing we're doing is noticeable to the staff!)
	server = url.host();
	apiToken = query.queryItemValue("api_token");
	this->generateAPILinkURL();

	// Put it in the settings and force a sync
	QSettings settings;
	settings.setValue("server", server);
	settings.setValue("apiToken", apiToken);
	settings.sync();

	// If we're in-game and expected to reload the page, do so.
	// This is NOT true when called from loadAPILink(), and should
	// be true when called from the menu item.
	if(reload)
		this->loadBundledIndex();
}

void KVMainWindow::openSettings()
{
	KVSettingsDialog *settingsDialog = new KVSettingsDialog(this);
	connect(settingsDialog, SIGNAL(apply()), SLOT(implementSettings()));
	connect(settingsDialog, SIGNAL(finished(int)), settingsDialog, SLOT(deleteLater()));
	settingsDialog->show();
}

void KVMainWindow::loadSettings(bool start)
{
	QSettings settings;

	server = settings.value("server").toString();
	apiToken = settings.value("apiToken").toString();

	if(server.isEmpty() || apiToken.isEmpty()) {
		this->askForAPILink(false);
		if(server.isEmpty() || apiToken.isEmpty())
			exit(0);
	}
	else this->generateAPILinkURL();

	qDebug() << "Server:" << server;
	qDebug() << "API Token:" << apiToken;
	qDebug() << "API Link:" << apiLink.toString();

	this->implementSettings(start);
}

void KVMainWindow::implementSettings(bool start)
{
	QSettings settings;

	bool translation = settings.value("viewerTranslation", kDefaultTranslation).toBool();
	if(translation != wvManager->translation) {
		wvManager->translation = translation;
		if(translation) loadTranslation();
		if(!start) loadBundledIndex();
	}

	KVTranslator::instance()->reportUntranslated = settings.value("reportUntranslated", kDefaultReportUntranslated).toBool();

	showTaskbarProgress = settings.value("taskbarProgress", kDefaultTaskbarProgress).toBool();

	if(settings.value("proxy", kDefaultProxy).toBool()) {
		wvManager->setProxy(QNetworkProxy(
								static_cast<QNetworkProxy::ProxyType>(settings.value("proxyType", kDefaultProxyType).toInt()),
								settings.value("proxyServer", kDefaultProxyServer).toString(),
								settings.value("proxyPort", kDefaultProxyPort).toInt(),
								settings.value("proxyUser", kDefaultProxyUser).toString(),
								settings.value("proxyPass", kDefaultProxyPass).toString()));
	} else {
		wvManager->setProxy(QNetworkProxy());
	}

	KVScreenshooter::instance().uploadScreenshots = settings.value("uploadScreenshots").toBool();
	KVScreenshooter::instance().screenshotsPath = settings.value("screenshotsPath").toString();
}

void KVMainWindow::clearCache()
{
	cache->clear();
	this->loadBundledIndex();
}

void KVMainWindow::showAbout()
{
	QMessageBox::about(this, "About KCTViewer",
					   QString(
						   "<h1>KCTViewer&nbsp;<small>%1</small></h1>"
						   ).arg(
						   QCoreApplication::applicationVersion()
						   )
					   );
}

void KVMainWindow::onVersionCheckFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

	// If the check failed, do nothing
	if(reply->error() != QNetworkReply::NoError)
		return;

	// Parse the version numbers
	QString newVersion = QString::fromUtf8(reply->readAll()).trimmed();
	QList<int> newVersionComponents;
	foreach(QString part, newVersion.split("."))
		newVersionComponents.append(part.toInt());

	QString appVersion = qApp->applicationVersion();
	QList<int> appVersionComponents;
	foreach(QString part, appVersion.split("."))
		appVersionComponents.append(part.toInt());

	// Compare component-per-component to see if we're outdated
	bool outdated = false;
	for(int i = 0; i < qMin(newVersionComponents.length(), appVersionComponents.length()); i++)
	{
		if(newVersionComponents[i] != appVersionComponents[i])
		{
			outdated = (newVersionComponents[i] > appVersionComponents[i]);
			break;
		}
	}

	// Display a message if we are
	if(outdated)
		QMessageBox::information(this, "New Version Available", "Version " + newVersion + " has been released, and is available at:<br /><a href=\"http://kancolletool.github.io/downloads/\">http://kancolletool.github.io/downloads/</a>");
}

void KVMainWindow::onLoadStarted()
{
	qDebug() << "Loading Started...";
}

void KVMainWindow::onLoadFinished(bool ok)
{
	qDebug() << "Finished Loading!" << ok;
	if(ok) this->setHTMLAPILink();
}

void KVMainWindow::onWaitingForTranslation()
{
	translationMsgBox = new QMessageBox(this);
	translationMsgBox->setWindowTitle("Waiting for Translation");
	translationMsgBox->setText("Waiting for the translation to load. This should only take a couple moments...");
	translationMsgBox->setStandardButtons(QMessageBox::NoButton);
	translationMsgBox->show();
}

void KVMainWindow::onTranslationLoaded()
{
	if(translationMsgBox) {
		translationMsgBox->accept();
		translationMsgBox->deleteLater();
		translationMsgBox = 0;
	}
}

void KVMainWindow::onTranslationLoadFailed(QString error)
{
	qDebug() << "Translation failed to load:" << error;

	QMessageBox::StandardButton button;
	if(KVTranslator::instance()->isLoaded()) {
		button = QMessageBox::warning(this, "Couldn't load network translation", "This might mean that your connection is bad. However, a cached translation has been loaded. Would you like to retry loading the translation from the network?", QMessageBox::Retry|QMessageBox::Ok, QMessageBox::Ok);
	} else {
		button = QMessageBox::warning(this, "Couldn't load translation", "This might mean that your connection is bad. You can continue without translation, but the game will be in Japanese.", QMessageBox::Retry|QMessageBox::Ok, QMessageBox::Ok);

		// Disable translation if they want it
		if(button != QMessageBox::Retry)
			wvManager->translation = false;
	}

	// To retry, just send the request again.
	if(button == QMessageBox::Retry)
		this->loadTranslation();
}

void KVMainWindow::onTrackedProgressChanged(qint64 progress, qint64 total)
{
	//qDebug() << "Progress:" << progress << "/" << total;
#ifdef Q_OS_WIN
	if(showTaskbarProgress && total > 0 && progress < total)
	{
		taskbarButton->progress()->show();

		// This is all to make sure it doesn't jump around all over the place,
		// since the game likes starting new requests halfways through loading
		// screens for some reason...
		int oldValue = taskbarButton->progress()->value();
		float fProgress = (float)progress;
		float fTotal = (float)total;
		float fValue = fProgress/fTotal;
		float newValue = fValue * 10000.0;
		//qDebug() << "Progress:" << oldValue << "->" << newValue;
		if(newValue > oldValue)
			taskbarButton->progress()->setValue(ceil(newValue));
	}
	else
	{
		taskbarButton->progress()->setValue(0);
		taskbarButton->progress()->hide();
	}
#else
	Q_UNUSED(progress)
	Q_UNUSED(total)
#endif
}

void KVMainWindow::setHTMLAPILink()
{
	qDebug() << "Updating web view credentials to" << server << "-" << apiToken;
	webView->page()->mainFrame()->evaluateJavaScript(QString("setCredentials(\"%1\", \"%2\"); null").arg(server, apiToken));
}

/*void KVMainWindow::onAPIError(KVProxyServer::APIStatus error)
{
	qDebug() << error;

	QString readableError;

	switch(error)
	{
		case KVProxyServer::OK:
			readableError = "No Error, you shouldn't be seeing this.";
			break;
		case KVProxyServer::Unauthorized:
			readableError = "Either your API Link is invalid, or it has expired. That happens sometimes.";
			break;
		case KVProxyServer::InvalidAPIVersion:
		case KVProxyServer::MissingParameters:
			readableError = "The game was updated, but you have an old client.";
			break;
		default:
			readableError = "An unknown error occurred. Please tell the developers that this happened ALONG WITH THE ERROR CODE, and what you think caused it.";
	}

	QMessageBox::critical(this, QString("Errorcat (Code %1)").arg((int)error), readableError);
}*/

void KVMainWindow::screenshot()
{
	KVScreenshooter::instance().takeScreenshot(webView);
}

void KVMainWindow::getAPILink()
{
	webView->setUrl(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/"));
}

void KVMainWindow::onGameFrameFinished(QUrl url)
{
	qDebug() << "Got new API url:" << url;
	QUrlQuery query(url);

	// Extract the important bits, and generate a well-formed URL from that
	// (It's important that nothing we're doing is noticeable to the staff!)
	server = url.host();
	apiToken = query.queryItemValue("api_token");
	this->generateAPILinkURL();

	// Put it in the settings and force a sync
	QSettings settings;
	settings.setValue("server", server);
	settings.setValue("apiToken", apiToken);
	settings.sync();

	this->loadBundledIndex();
}
