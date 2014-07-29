#ifndef KVSCREENSHOOTER_H
#define KVSCREENSHOOTER_H

#include <QObject>
#include <QNetworkAccessManager>

/**
 * @brief The class for make, save or upload screenshots
 */
class KVScreenshooter : public QObject
{
	Q_OBJECT

public:
	static KVScreenshooter& instance();

	/**
	 * @brief Function for taking screenshot of the passed widget
	 * @param widget Widget that will be captured
	 * @param uploadScreenshots Upload screenshot on imgur instead of saving it locally
	 */
	void takeScreenshot(QWidget *widget, bool uploadScreenshots);

private:
	KVScreenshooter(QObject *parent = 0);
	KVScreenshooter(const KVScreenshooter&);
	KVScreenshooter& operator=(const KVScreenshooter&);
	virtual ~KVScreenshooter();

protected:
	QNetworkAccessManager *manager;

	/**
	 * @brief Function for capturing screenshots
	 * @param widget Widget that will be captured
	 * @return Captured screenshot in Qimage objest
	 */
	QImage captureScreenshot(QWidget *widget);

	/**
	 * @brief Function for uploading screenshots
	 * @param image Captured screenshot in Qimage objest
	 */
	void uploadScreenshot(QImage image);

	/**
	 * @brief Function for saving screenshots
	 * @param image Captured screenshot in Qimage objest
	 */
	void saveScreenshot(QImage image);

private slots:
	void uploadingFinished(QNetworkReply* reply);

};

#endif // KVSCREENSHOOTER_H
