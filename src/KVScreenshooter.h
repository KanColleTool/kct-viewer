#ifndef KVSCREENSHOOTER_H
#define KVSCREENSHOOTER_H

#include <QObject>
#include <QNetworkAccessManager>

class KVScreenshooter : public QObject
{
	Q_OBJECT

public:
	static KVScreenshooter& instance();

	void takeScreenshot(QWidget *widget);

	bool uploadScreenshots;

private:
	KVScreenshooter(QObject *parent = 0);
	KVScreenshooter(const KVScreenshooter&);
	KVScreenshooter& operator=(const KVScreenshooter&);
	virtual ~KVScreenshooter();

protected:
	QNetworkAccessManager *manager;

	QImage captureScreenshot(QWidget *widget);
	void uploadScreenshot(QImage image);
	void saveScreenshot(QImage image);

private slots:
	void uploadingFinished(QNetworkReply* reply);

};

#endif // KVSCREENSHOOTER_H
