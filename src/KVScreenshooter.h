#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QClipboard>

class KVScreenshooter : public QObject
{
	Q_OBJECT
public:
	explicit KVScreenshooter(QObject *parent = 0);
	void takeScreenshot(QWidget *widget);

protected:
	QNetworkAccessManager *manager;
	QClipboard *clipboard;

	QImage captureScreenshot(QWidget *widget);
	void uploadScreenshot(QImage image);
	void saveScreenshot(QImage image);

private slots:
	void onFinished(QNetworkReply* reply);

};

#endif // UPLOADER_H
