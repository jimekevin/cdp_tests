#include <QtWidgets/QWidget>
//#include <QtGui/QWindow>
#include <QtCore/QDebug>

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
	connect(ui->mainGLWidget, &MainGLWidget::hideRecordVideoButton, [this]() {
		ui->recordVideoPushButton->setDisabled(true);
	});

	connect(ui->recordVideoPushButton, SIGNAL(clicked()), ui->mainGLWidget, SLOT(recordVideo()));
	//connect(ui->mainGLWidget, SIGNAL(startedRecordingVideo()), ui->recordVideoPushButton, SLOT(startRecordingVideo()));
	//connect(ui->mainGLWidget, SIGNAL(stoppedRecordingVideo()), ui->recordVideoPushButton, SLOT(stopRecordingVideo()));
	connect(ui->mainGLWidget, &MainGLWidget::startedRecordingVideo, [this]() {
		ui->recordVideoPushButton->setText("Stop Recording");
	});
	connect(ui->mainGLWidget, &MainGLWidget::stoppedRecordingVideo, [this]() {
		ui->recordVideoPushButton->setText("Start Recording");
	});
	connect(ui->compressVideoRecordingCheckbox, SIGNAL(stateChanged(int)), ui->mainGLWidget, SLOT(enableVideoRecordingCompression(int)));

	// Distance threshold sliders
#define MAKE_SLIDER_CONNECTION(VARIABLE, TEXT, SLIDER, TASK, LABEL, SCALE) \
	connect(ui->SLIDER, &QSlider::valueChanged, [this](int value) {        \
		float fvalue = value / SCALE;                                      \
		ui->mainGLWidget->TASK->VARIABLE = fvalue;                         \
		std::stringstream ss;                                              \
		ss << TEXT << fvalue;                                              \
		ui->LABEL->setText(QString(ss.str().c_str()));                     \
	});                                                                    \
	ui->SLIDER->setValue((int)(ui->mainGLWidget->TASK->VARIABLE * SCALE))

	// Threshold
	MAKE_SLIDER_CONNECTION(minX, "Min X: ", thresholdMinXSlider, thresholdFilter, thresholdMinXLabel, 10.0f);
	MAKE_SLIDER_CONNECTION(maxX, "Max X: ", thresholdMaxXSlider, thresholdFilter, thresholdMaxXLabel, 10.0f);
	MAKE_SLIDER_CONNECTION(minY, "Min Y: ", thresholdMinYSlider, thresholdFilter, thresholdMinYLabel, 10.0f);
	MAKE_SLIDER_CONNECTION(maxY, "Max Y: ", thresholdMaxYSlider, thresholdFilter, thresholdMaxYLabel, 10.0f);
	MAKE_SLIDER_CONNECTION(minZ, "Min Z: ", thresholdMinZSlider, thresholdFilter, thresholdMinZLabel, 10.0f);
	MAKE_SLIDER_CONNECTION(maxZ, "Max Z: ", thresholdMaxZSlider, thresholdFilter, thresholdMaxZLabel, 10.0f);

	// Contour
	MAKE_SLIDER_CONNECTION(threshold1, "Contour 1: ", contourThreshold1Slider, contourDetector, contourThreshold1Label, 100.0f);
	MAKE_SLIDER_CONNECTION(threshold2, "Contour 2: ", contourThreshold2Slider, contourDetector, contourThreshold2Label, 100.0f);

	connect(ui->mainGLWidget, &MainGLWidget::sliderValuesChanged, [this]() {
		ui->thresholdMinXSlider->setValue(ui->mainGLWidget->thresholdFilter->minX * 10.0f);
		ui->thresholdMaxXSlider->setValue(ui->mainGLWidget->thresholdFilter->maxX * 10.0f);
		ui->thresholdMinYSlider->setValue(ui->mainGLWidget->thresholdFilter->minY * 10.0f);
		ui->thresholdMaxYSlider->setValue(ui->mainGLWidget->thresholdFilter->maxY * 10.0f);
		ui->thresholdMinZSlider->setValue(ui->mainGLWidget->thresholdFilter->minZ * 10.0f);
		ui->thresholdMaxZSlider->setValue(ui->mainGLWidget->thresholdFilter->maxZ * 10.0f);
		ui->contourThreshold1Slider->setValue(ui->mainGLWidget->contourDetector->threshold1 * 100.0f);
		ui->contourThreshold2Slider->setValue(ui->mainGLWidget->contourDetector->threshold2 * 100.0f);
	});

#undef MAKE_SLIDER_CONNECTION

	// Export settings
	connect(ui->exportSettingsButton, SIGNAL(clicked()), ui->mainGLWidget, SLOT(exportSettings()));
}

MainWindow::~MainWindow()
{
	delete ui;
	KM.terminate();
}
