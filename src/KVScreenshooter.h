#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QApplication>
#include <QBuffer>
#include <QStandardPaths>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QSettings>

class KVScreenshooter : public QObject
{
	Q_OBJECT
public:
	explicit KVScreenshooter(QObject *parent = 0);
	void takeScreenshot(QWidget *widget);

private:
	QNetworkAccessManager *manager;
	QClipboard *clipboard;

	QImage captureScreenshot(QWidget *widget);
	void uploadScreenshot(QImage image);
	void saveScreenshot(QImage image);

signals:

public slots:

private slots:
	void onFinished(QNetworkReply* reply);

};

#endif // UPLOADER_H
