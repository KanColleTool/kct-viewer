#include "KVMainWindow.h"
#include "ui_KVMainWindow.h"
#include <QWebFrame>

KVMainWindow::KVMainWindow(QWidget *parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	ui(new Ui::KVMainWindow)
{
	ui->setupUi(this);
	
	this->setup();
	this->autoLockWindowSize();
	this->connectSignals();
	
	this->startGame();
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
	
	// The context menu only contains "Reload" anyways
	webView->setContextMenuPolicy(Qt::PreventContextMenu);
	
	// These are so large that they create a need for themselves >_>
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
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
	server = settings.value("server").toString();
	apiToken = settings.value("apiToken").toString();
	
	return (!server.isEmpty() && !apiToken.isEmpty());
}

void KVMainWindow::startGame()
{
	this->webView->setUrl(QUrl("qrc:/index.html"));
}



void KVMainWindow::onWebViewLoadFinished(bool success)
{
	if (!success) {
		qWarning() << "Page failed to load!";
		return;
	}
	
	qDebug() << "Page successfully loaded";
}



void KVMainWindow::on_actionEnterAPILink_triggered()
{
	
}

void KVMainWindow::on_actionSettings_triggered()
{
	
}

void KVMainWindow::on_actionClearCache_triggered()
{
	
}

void KVMainWindow::on_actionReset_triggered()
{
	
}

void KVMainWindow::on_actionExit_triggered()
{
	qApp->quit();
}

void KVMainWindow::on_actionAbout_triggered()
{
	
}

void KVMainWindow::on_actionScreenshot_triggered()
{
	
}

void KVMainWindow::on_actionGetAPILink_triggered()
{
	
}
