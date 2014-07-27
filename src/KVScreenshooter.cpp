#include "KVScreenshooter.h"
#include "KVDefaults.h"
#include "KVNotificationCenter.h"

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QBuffer>
#include <QStandardPaths>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDebug>

KVScreenshooter::KVScreenshooter(QObject *parent) :
	QObject(parent)
{
	manager = new QNetworkAccessManager(this);
	clipboard = QApplication::clipboard();
}

QImage KVScreenshooter::captureScreenshot(QWidget *widget)
{
	QImage image(widget->size(), QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);

	widget->render(&painter);
	painter.end();

	return image;
}

void KVScreenshooter::saveScreenshot(QImage image)
{
	QDateTime currentTime = currentTime.currentDateTime();
	QString currentTimeString = currentTime.toString("yyyy-MM-dd hh:mm:ss");

	QString picName = QString("%1 %2.png").arg("Screenshot", currentTimeString);

	QString picsPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

	int counter = 0;
	while (QFile(QDir(picsPath).filePath(picName)).exists()) {
		counter++;
		picName = QString("%1 %2-%3.png").arg("Screenshot", currentTimeString, QString::number(counter));
	}

	QString fullPath = QDir(picsPath).filePath(picName);

	image.save(fullPath, "PNG");
}

void KVScreenshooter::uploadScreenshot(QImage image)
{
	QByteArray rawData;
	QBuffer buffer(&rawData);
	buffer.open(QBuffer::WriteOnly);

	image.save(&buffer, "PNG");
	buffer.close();

	QNetworkRequest request(QUrl("https://api.imgur.com/3/image.json"));
	request.setRawHeader("Authorization", "Client-ID ef6bd901726e8b7");

	QByteArray requestBody;
	requestBody.append(QString("image=").toUtf8());
	requestBody.append(QUrl::toPercentEncoding(rawData.toBase64()));

	manager->post(request, requestBody);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
}

void KVScreenshooter::onFinished(QNetworkReply *reply)
{
	if (reply->error()) {
		qDebug() << "Error:" << reply->errorString();
		return;
	}

	QString textData(reply->readAll());

	QJsonDocument jsonDocument = QJsonDocument::fromJson(textData.toUtf8());
	QJsonObject jsonObject = jsonDocument.object();

	QString link = jsonObject["data"].toObject()["link"].toString();

	clipboard->setText(link);

	reply->deleteLater();
}

void KVScreenshooter::takeScreenshot(QWidget *widget)
{
	QSettings settings;

	QImage image = KVScreenshooter::captureScreenshot(widget);

	if (settings.value("uploadeScreenshot", kDefaultUploadScreenshots).toBool()) {
		KVScreenshooter::uploadScreenshot(image);
	} else {
		KVScreenshooter::saveScreenshot(image);
	}
}
