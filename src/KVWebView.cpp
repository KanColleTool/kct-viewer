#include "KVWebView.h"
#include <QMouseEvent>

KVWebView::KVWebView(QWidget *parent) :
	QWebView(parent)
{
}

void KVWebView::mousePressEvent(QMouseEvent *ev)
{
	// Strip out the right button from events
	if(ev->buttons() & Qt::RightButton)
	{
		ev->accept();
		return;
	}

	QWebView::mousePressEvent(ev);
}
