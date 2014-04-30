#ifndef KVWEBVIEW_H
#define KVWEBVIEW_H

#include <QWebView>

class KVWebView : public QWebView
{
	Q_OBJECT

public:
	explicit KVWebView(QWidget *parent = 0);

protected:
	virtual void mousePressEvent(QMouseEvent *ev);
};

#endif // KVWEBVIEW_H
