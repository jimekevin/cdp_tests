#include <iostream>
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>

#include "MainWindow.h"
#include "Config.h"

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

#ifdef APPLE
	QSurfaceFormat format;
	format.setVersion(4, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setOption(QSurfaceFormat::DebugContext);
	QSurfaceFormat::setDefaultFormat(format);
#endif

	auto conf = Config::instance();
	conf.parseSimple("config/default.yaml");
	auto inputMode = conf.getValueI("input");
	auto inputSource = conf.getValue("input_source");
	if (inputMode == 2) {
		if (KinectManager::instance().initializeFromFile(inputSource) != S_OK) {
			std::cout << "Failed loading input stream from file" << std::endl;
			return 0;
		}
	}
	else {
		if (KinectManager::instance().initialize() != S_OK) {
			std::cout << "Failed loading input stream from Kinect device" << std::endl;
			return 0;
		}
	}

	MainWindow window;
	window.show();

	app.exec();

	return 0;
}
