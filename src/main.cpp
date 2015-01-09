#include "KVMainWindow.h"
#include "version.h"

#include <QApplication>
#include <QWebSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("KanColleTool");
	QCoreApplication::setApplicationVersion(KCT_VERSION);
	QCoreApplication::setOrganizationName("KanColleTool");

	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);

	//QStringList args = QCoreApplication::arguments();

	KVMainWindow w;
	w.show();

	return a.exec();
}
