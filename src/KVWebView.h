#ifndef KVWEBVIEW_H
#define KVWEBVIEW_H

#include <QWebView>

/**
 * Modified web view.
 */
class KVWebView : public QWebView
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 * 
	 * @param  parent Parent widget
	 */
	explicit KVWebView(QWidget *parent = 0);

protected:
	/**
	 * Intercepts mouse presses to ignore right clicks.
	 * 
	 * @param ev Mouse event
	 */
	virtual void mousePressEvent(QMouseEvent *ev);

signals:
	/**
	 * The game frame finished loading.
	 * 
	 * \todo Rename this.
	 * 
	 * @param url The URL for mainD2.swf
	 */
	void gameFrameFinished(QUrl url);

private slots:
	/**
	 * Called when a frame is created.
	 */
	void onFrameCreated(QWebFrame *frame);
	
	/**
	 * Called when a frame finishes loading.
	 */
	void onGameFrameLoadFinished(bool ok);
};

#endif // KVWEBVIEW_H
