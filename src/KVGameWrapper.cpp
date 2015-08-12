#include "KVGameWrapper.h"
#include "KVNetworkAccessManager.h"
#include "KVNetworkReply.h"
#include "KVTranslator.h"
#include <QNetworkDiskCache>
#include <QUrlQuery>
#include <QFile>
#include <QWebFrame>
#include <QStandardPaths>

KVGameWrapper::KVGameWrapper(QObject *parent):
	QObject(parent),
	m_page(new QWebPage(this))
{
	KVNetworkAccessManager *manager = new KVNetworkAccessManager(m_page);
	m_page->setNetworkAccessManager(manager);
	connect(manager, SIGNAL(readyToPostProcess(KVNetworkReply*)), this, SLOT(onInterceptedRequestReadyToPostProcess(KVNetworkReply*)));
	
	QNetworkDiskCache *cache = new QNetworkDiskCache(this);
	cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setMaximumCacheSize(1*1024*1024*1024);
	manager->setCache(cache);
	
	connect(m_page, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoadFinished(bool)));
}

KVGameWrapper::~KVGameWrapper()
{
	
}



QUrl KVGameWrapper::apiLink() const
{
	return QUrl(QString("http://%1/kcs/mainD2.swf?api_token=%2").arg(this->server(), this->apiToken()));
}

void KVGameWrapper::setApiLink(const QUrl &url)
{
	this->setServer(url.host());
	this->setApiToken(QUrlQuery(url).queryItemValue("api_token"));
}



QString KVGameWrapper::server() const { return m_server; }
void KVGameWrapper::setServer(const QString &v) { m_server = v; emit serverChanged(); }

QString KVGameWrapper::apiToken() const { return m_apiToken; }
void KVGameWrapper::setApiToken(const QString &v) { m_apiToken = v; emit apiTokenChanged(); }

QWebPage* KVGameWrapper::page() const { return m_page; }



void KVGameWrapper::startGame()
{
	QFile file(":/index.html");
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning("Can't open compiled-in index.html");
		return;
	}
	
	m_page->mainFrame()->setHtml(file.readAll(), this->apiLink());
}

void KVGameWrapper::onPageLoadFinished(bool ok)
{
	if (!ok) {
		qWarning() << "Page failed to load!";
		return;
	}
	
	qDebug() << "Page successfully loaded";
	
	QUrl link = this->apiLink();
	QString script = QString("loadGame(\"%1\");").arg(link.toString());
	m_page->mainFrame()->evaluateJavaScript(script);
}

void KVGameWrapper::onInterceptedRequestReadyToPostProcess(KVNetworkReply *reply)
{
	qDebug() << "Intercepted Request:" << reply->url();
	QByteArray data = reply->data();
	
	QByteArray prefix = "svdata=";
	if(data.startsWith(prefix)) {
		QJsonDocument doc = QJsonDocument::fromJson(data.mid(prefix.size()));
		doc = KVTranslator::instance().translate(doc);
		QByteArray newData = prefix + doc.toJson(QJsonDocument::Compact);
		reply->setData(newData);
	}
}
