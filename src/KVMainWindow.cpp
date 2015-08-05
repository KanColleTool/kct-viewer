#include "KVMainWindow.h"
#include "ui_KVMainWindow.h"
#include "KVNetworkAccessManager.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QNetworkDiskCache>
#include <QWebFrame>
#include <QSettings>
#include <QUrlQuery>
#include <QFile>

KVMainWindow::KVMainWindow(QWidget *parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	ui(new Ui::KVMainWindow)
{
	ui->setupUi(this);
	
	this->setup();
	this->autoLockWindowSize();
	this->connectSignals();
	
	this->loadCredentials();
	this->startGame();
}

KVMainWindow::~KVMainWindow()
{
	
}



QUrl KVMainWindow::apiLink() const
{
	return QUrl(QString("http://%1/kcs/mainD2.swf?api_token=%2").arg(this->server(), this->apiToken()));
}

void KVMainWindow::setApiLink(const QUrl &url)
{
	this->setServer(url.host());
	this->setApiToken(QUrlQuery(url).queryItemValue("api_token"));
}



QString KVMainWindow::server() const { return m_server; }
void KVMainWindow::setServer(const QString &v) { m_server = v; emit serverChanged(); }

QString KVMainWindow::apiToken() const { return m_apiToken; }
void KVMainWindow::setApiToken(const QString &v) { m_apiToken = v; emit apiTokenChanged(); }



void KVMainWindow::setup()
{
	// It's not possible to set up QWebView subclasses in the designer
	webView = new KVWebView(this);
	webView->setFixedSize(800, 480);
	this->centralWidget()->layout()->addWidget(webView);
	
	// The context menu only contains "Reload" anyways
	webView->setContextMenuPolicy(Qt::PreventContextMenu);
	
	// These are so large that they create a need for themselves >_>
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	
	// Use a custom network manager for the web view
	KVNetworkAccessManager *wvManager = new KVNetworkAccessManager(this);
	webView->page()->setNetworkAccessManager(wvManager);
	
	// Use a 1GB-capped disk cache
	QNetworkDiskCache *cache = new QNetworkDiskCache(this);
	cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setMaximumCacheSize(1*1024*1024*1024);
	wvManager->setCache(cache);
}

void KVMainWindow::connectSignals()
{
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(onWebViewLoadFinished(bool)));
}

void KVMainWindow::autoLockWindowSize()
{
	this->adjustSize();
	this->setFixedSize(this->width(), this->height());
}



bool KVMainWindow::loadCredentials()
{
	QSettings settings;
	setServer(settings.value("server").toString());
	setApiToken(settings.value("apiToken").toString());
	
	return (!server().isEmpty() && !apiToken().isEmpty());
}

void KVMainWindow::storeCredentials()
{
	QSettings settings;
	settings.setValue("server", server());
	settings.setValue("apiToken", apiToken());
}

void KVMainWindow::startGame()
{
	QFile file(":/index.html");
	if (!file.open(QIODevice::ReadOnly)) {
		qFatal("Can't open compiled-in index.html");
	}
	
	webView->setHtml(file.readAll(), this->apiLink());
}



void KVMainWindow::onWebViewLoadFinished(bool success)
{
	if (!success) {
		qWarning() << "Page failed to load!";
		return;
	}
	
	qDebug() << "Page successfully loaded";
	
	QUrl link = this->apiLink();
	QString script = QString("loadGame(\"%1\");").arg(link.toString());
	webView->page()->mainFrame()->evaluateJavaScript(script);
}



void KVMainWindow::on_actionEnterAPILink_triggered()
{
	
}

void KVMainWindow::on_actionSettings_triggered()
{
	
}

void KVMainWindow::on_actionClearCache_triggered()
{
	webView->page()->networkAccessManager()->cache()->clear();
	this->startGame();
}

void KVMainWindow::on_actionReset_triggered()
{
	this->startGame();
}

void KVMainWindow::on_actionExit_triggered()
{
	qApp->quit();
}

void KVMainWindow::on_actionAbout_triggered()
{
	QString body = QString("<h1>KCTViewer&nbsp;<small>%1</small></h1>")
		.arg(QCoreApplication::applicationVersion());
	
	QMessageBox::about(this, "About KCTViewer", body);
}

void KVMainWindow::on_actionScreenshot_triggered()
{
	
}

void KVMainWindow::on_actionGetAPILink_triggered()
{
	
}
