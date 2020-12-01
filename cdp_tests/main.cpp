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

	//QSurfaceFormat format;
	//format.setDepthBufferSize(24);
	//format.setSamples(4);
	//format.setVersion(4, 1);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	//format.setRenderableType(QSurfaceFormat::OpenGL);
	//QSurfaceFormat::setDefaultFormat(format);

	KinectManager::instance().init();

	MainWindow window;
	//window.setFormat(format);
	window.resize(QSize(800, 600));
	window.show();

	app.exec();

	return 0;
}