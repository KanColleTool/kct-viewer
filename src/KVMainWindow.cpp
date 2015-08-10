#include "KVMainWindow.h"
#include "ui_KVMainWindow.h"
#include <QMessageBox>
#include <QWebFrame>
#include <QAbstractNetworkCache>
#include <QSettings>

KVMainWindow::KVMainWindow(QWidget *parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	ui(new Ui::KVMainWindow)
{
	ui->setupUi(this);
	
	this->setup();
	this->autoLockWindowSize();
	
	this->loadCredentials();
	game->startGame();
}

KVMainWindow::~KVMainWindow()
{
	
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
	
	// The context menu only contains "Reload" anyways
	webView->setContextMenuPolicy(Qt::PreventContextMenu);
	
	// These are so large that they create a need for themselves >_>
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
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
