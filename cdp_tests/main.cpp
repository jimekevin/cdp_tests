#include <iostream>
#include <opencv2/imgproc.hpp>
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include "window.h"
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
	//format.setDepthBufferSize(24);
	//format.setSamples(4);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	QSurfaceFormat::setDefaultFormat(format);

	KinectManager::instance().init();

	Window window;
	window.setFormat(format);
	window.resize(QSize(800, 600));
	window.show();

	app.exec();

	return 0;
}