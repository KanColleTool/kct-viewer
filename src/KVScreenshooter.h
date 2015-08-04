#ifndef KVSCREENSHOOTER_H
#define KVSCREENSHOOTER_H

#include <QObject>
#include <QNetworkAccessManager>

/**
 * The class for make, save or upload screenshots
 */
class KVScreenshooter : public QObject
{
	Q_OBJECT

public:
	/**
	 * Returns a shared instance.
	 */
	static KVScreenshooter& instance();

	/**
	 * Function for taking screenshot of the passed widget
	 * @param widget Widget that will be captured
	 */
	void takeScreenshot(QWidget *widget);



	bool uploadScreenshots;			///< Should screenshots be uploaded?
	QString screenshotsPath;		///< Where should we save images?
	QString screenshotsFormat;		///< What format should they be in? \todo Enums, dammit!

protected:
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
	
	/// Network access manager
	QNetworkAccessManager *manager;

protected slots:
	/**
	 * Called when an upload finishes.
	 */
	void uploadingFinished();

private:
	// Singleton stuff
	KVScreenshooter(QObject *parent = 0);
	KVScreenshooter(const KVScreenshooter&);
	KVScreenshooter& operator=(const KVScreenshooter&);
	virtual ~KVScreenshooter();

};

#endif // KVSCREENSHOOTER_H
