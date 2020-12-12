#include <iostream>
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>

#include "MainWindow.h"

#ifdef APPLE
#include "KinectManager_MacOS.h"
#else
#include "KinectManager_Windows.h"
#endif

int main(int argc, char **argv) {

	QApplication app(argc, argv);
	QApplication::setOrganizationName("LS AI");
	QApplication::setOrganizationName("cdp.ai.ar.tum.de");
	QApplication::setApplicationName("Collaborative Design Platform");

    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(format);

    KinectManager::instance().initialize();

	MainWindow window;
	window.show();

	app.exec();

	return 0;
}