#include <QtWidgets/QWidget>
//#include <QtGui/QWindow>

#include "MainWindow.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->mainGLWidget, SIGNAL(logMessage(std::string)), ui->outputTextBrowser, SLOT(logEntry(std::string)));
	connect(ui->mainGLWidget, SIGNAL(setOutput(QString)), ui->outputTextBrowser, SLOT(setPlainText(QString)));

	//connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
	//connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
	//connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
	delete ui;
}
