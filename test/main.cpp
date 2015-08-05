#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTimer::singleShot(0, [=] {
		int result = Catch::Session().run(argc, argv);
		qApp->exit(result);
	});
	return a.exec();
}
