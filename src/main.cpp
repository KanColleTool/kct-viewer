#include "KVMainWindow.h"
#include "version.h"

#include <QApplication>
#include <QWebSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("KanColleTool");
	QCoreApplication::setApplicationVersion(KCT_VERSION);
	QCoreApplication::setOrganizationName("KanColleTool");

	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);

	QStringList args = QCoreApplication::arguments();

	KVMainWindow *win = new KVMainWindow();
	win->show();

	int ret = app.exec();
	delete win;

	return ret;
}
