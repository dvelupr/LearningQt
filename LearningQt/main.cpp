#include "qtopencvwidget.hpp"
#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include <qthread.h>

#define	USE_SPLASH_SCREEN

#ifdef USE_SPLASH_SCREEN
class I : public QThread
{
public:
	static void sleep(unsigned long secs) { QThread::sleep(secs); }
};
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#ifdef USE_SPLASH_SCREEN
	QPixmap pixmap("Images/splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
#endif

	QtOpencvWidget cvWidget;

#ifdef USE_SPLASH_SCREEN
	I::sleep(1); // splash is shown for at least [this many] second(s)
#endif

	cvWidget.show();

#ifdef USE_SPLASH_SCREEN
	splash.finish(&cvWidget);
#endif

	return a.exec();
}
