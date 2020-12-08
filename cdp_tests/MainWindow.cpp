#include <QtWidgets/QWidget>
//#include <QtGui/QWindow>

#include "MainWindow.h"
#include "ui_main_window.h"

#ifdef APPLE
#include "KinectManager_MacOS.h"
#else
#include "KinectManager_Windows.h"
#endif

#define KM KinectManager::instance()

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->mainGLWidget, SIGNAL(logMessage(std::string)), ui->outputTextBrowser, SLOT(logEntry(std::string)));
	connect(ui->mainGLWidget, SIGNAL(setOutput(QString)), ui->outputTextBrowser, SLOT(setPlainText(QString)));

	connect(ui->recordVideoPushButton, SIGNAL(clicked()), ui->mainGLWidget, SLOT(recordVideo()));
	connect(ui->mainGLWidget, SIGNAL(startedRecordingVideo()), ui->recordVideoPushButton, SLOT(startRecordingVideo()));
	connect(ui->mainGLWidget, SIGNAL(stoppedRecordingVideo()), ui->recordVideoPushButton, SLOT(stopRecordingVideo()));

	connect(ui->minDistanceSlider, &QSlider::valueChanged, [this](int value) {
		float minDist = value / 100.0f;
		ui->mainGLWidget->thresholdFilter->updateMinDistance(minDist);
		std::stringstream ss;
		ss << "Min distance: " << minDist;
		ui->minDistanceLabel->setText(QString(ss.str().c_str()));
	});
	connect(ui->maxDistanceSlider, &QSlider::valueChanged, [this](int value) {
		float maxDist = value / 100.0f;
		ui->mainGLWidget->thresholdFilter->updateMaxDistance(maxDist);
		std::stringstream ss;
		ss << "Max distance: " << maxDist;
		ui->maxDistanceLabel->setText(QString(ss.str().c_str()));
	});
	ui->minDistanceSlider->setValue(ui->mainGLWidget->thresholdFilter->getMinDistance() * 100);
	ui->maxDistanceSlider->setValue(ui->mainGLWidget->thresholdFilter->getMaxDistance() * 100);
}

MainWindow::~MainWindow()
{
	delete ui;
	KM.terminate();
}
