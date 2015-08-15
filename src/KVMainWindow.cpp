#include "KVMainWindow.h"
#include "ui_KVMainWindow.h"
#include "KVTranslator.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include <QWebFrame>
#include <QNetworkDiskCache>
#include <QSettings>
#include <QDir>

KVMainWindow::KVMainWindow(QWidget *parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	untranslated("orig"),
	ui(new Ui::KVMainWindow)
{
	ui->setupUi(this);
	
	this->setup();
	this->connectSignals();
	this->autoLockWindowSize();
	
	this->loadCredentials();
	game->startGame();
}

KVMainWindow::~KVMainWindow()
{
	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	untranslated.save(cacheDir.absoluteFilePath("untranslated.json"));
}



void KVMainWindow::setup()
{
	// It's not possible to set up QWebView subclasses in the designer
	webView = new KVWebView(this);
	webView->setFixedSize(800, 480);
	this->centralWidget()->layout()->addWidget(webView);
	
	// Set up the game wrapper and use its page
	game = new KVGameWrapper(this);
	webView->setPage(game->page());
	
	// Set up the cache and stuff
	QNetworkDiskCache *cache = new QNetworkDiskCache(this);
	cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setMaximumCacheSize(1*1024*1024*1024);
	game->page()->networkAccessManager()->setCache(cache);
	
	// The context menu only contains "Reload" anyways
	webView->setContextMenuPolicy(Qt::PreventContextMenu);
	
	// These are so large that they create a need for themselves >_>
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
}

void KVMainWindow::connectSignals()
{
	connect(&KVTranslator::instance(), SIGNAL(missingTranslation(const QString&, const QString&, const QUrl&)),
		this, SLOT(onMissingTranslation(const QString&, const QString&, const QUrl&)));
}

void KVMainWindow::autoLockWindowSize()
{
	this->adjustSize();
	this->setFixedSize(this->width(), this->height());
}



bool KVMainWindow::loadCredentials()
{
	QSettings settings;
	game->setServer(settings.value("server").toString());
	game->setApiToken(settings.value("apiToken").toString());
	
	qDebug() << "Loaded Credentials:" << game->server() << game->apiToken();
	
	return (!game->server().isEmpty() && !game->apiToken().isEmpty());
}

void KVMainWindow::storeCredentials()
{
	QSettings settings;
	settings.setValue("server", game->server());
	settings.setValue("apiToken", game->apiToken());
}



void KVMainWindow::on_actionEnterAPILink_triggered()
{
	QString text = QString("<p>Please enter your API Link.</p>") +
					QString("<p>") + 
						QString("It should look something like:<br />") + 
						QString("<code>http://125.6.XXX.XXX/kcs/mainD2.swf?api_token=xxxxxxxxxx...</code>") +
					QString("</p>");
	QString link = QInputDialog::getText(this, "Enter API Link", text);
	
	if (!link.isEmpty()) {
		game->setApiLink(link);
		game->startGame();
		this->storeCredentials();
	}
}

void KVMainWindow::on_actionSettings_triggered()
{
	
}

void KVMainWindow::on_actionClearCache_triggered()
{
	webView->page()->networkAccessManager()->cache()->clear();
	game->startGame();
}

void KVMainWindow::on_actionReset_triggered()
{
	game->startGame();
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

void KVMainWindow::onMissingTranslation(const QString &phrase, const QString &key, const QUrl &source)
{
	QMap<QString,QVariant> data {
		{ "src", source.path() },
		{ "ctx", key },
		{ "orig", phrase },
	};
	untranslated.add(data);
}
