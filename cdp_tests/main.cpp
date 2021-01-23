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
    switch (conf.getValueI("video")) {
        case 0:
            KinectManager::instance().initialize();
            break;
        case 1:
            auto videoSource = conf.getValue("video_source");
            KinectManager::instance().initialize(videoSource);
            break;
    }

	MainWindow window;
	window.show();

	app.exec();

	return 0;
}
