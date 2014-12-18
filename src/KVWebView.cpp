#include "KVWebView.h"

#include <QMouseEvent>
#include <QWebFrame>
#include <QWebElement>
#include <QTimer>

KVWebView::KVWebView(QWidget *parent) :
	QWebView(parent)
{
	connect(this->page(), SIGNAL(frameCreated(QWebFrame*)), this, SLOT(onFrameCreated(QWebFrame*)));
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



void KVWebView::onFrameCreated(QWebFrame *frame)
{
	if(frame->frameName() == "game_frame")
	{
		connect(frame, &QWebFrame::loadFinished, [=](bool ok){
			qDebug() << "Game frame loaded!" << ok;

			QTimer *timer = new QTimer(this);
			connect(timer, &QTimer::timeout, [=]{
				if(!frame->findFirstElement("embed#externalswf").isNull())
				{
					// Make an URL
					QUrl url(frame->findFirstElement("embed#externalswf").attribute("src"));
					emit gameFrameFinished(url);

					timer->stop();
					timer->deleteLater();
				}
				else
				{
					qDebug() << " -> nope, no externalswf yet";
				}
			});
			timer->start(100);
		});
	}
}
