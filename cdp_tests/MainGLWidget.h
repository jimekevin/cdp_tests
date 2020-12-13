#ifndef MAIN_GL_WIDGET_H
#define MAIN_GL_WIDGET_H

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLDebugLogger>
#include <QtWidgets/QOpenGLWidget>

#include "Pipeline.h"
#ifdef APPLE
#include "KinectManager_MacOS.h"
#else
#include "KinectManager_Windows.h"
#endif
#include "Map.h"
#include "filters/ThresholdFilter.h"
#include "filters/ContourFilter.h"

class QOpenGLShaderProgram;

class MainGLWidget : public QOpenGLWidget,
	protected QOpenGLFunctions
{
	Q_OBJECT

		// OpenGL Events
public:
	explicit MainGLWidget(QWidget *parent = 0);
	~MainGLWidget();

	// Threshold
	float minDistance = 0.5f;
	float maxDistance = 3.0f;
	ThresholdFilter *thresholdFilter = new ThresholdFilter(minDistance, maxDistance);

	// Contour
	float contourThreshold1 = 0.8f;
	float contourThreshold2 = 1.0f;
	ContourFilter *contourFilter = new ContourFilter(contourThreshold1, contourThreshold2);

public slots:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void teardownGL();

	void keyPressEvent(QKeyEvent *event);
	void timerEvent(QTimerEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void recordVideo();

signals:
	void logMessage(std::string message);
	void setOutput(QString output);
	void startedRecordingVideo();
	void stoppedRecordingVideo();

private:
	// OpenGL State Information
	//QOpenGLBuffer m_vertex;
	//QOpenGLVertexArrayObject m_object;
	//QOpenGLShaderProgram *m_program;

	QOpenGLShaderProgram *kinectProgram;
	QOpenGLVertexArrayObject kinectVAO;
	QOpenGLBuffer kinectBuffer;
	QOpenGLBuffer kinectDepthBuffer;
	QOpenGLBuffer kinectRGBBuffer;

	Pipeline pipeline;

	cv::Mat depthMat = cv::Mat(KinectManager::HEIGHT, KinectManager::WIDTH, CV_32FC3);
	cv::Mat rgbMat   = cv::Mat(KinectManager::HEIGHT, KinectManager::WIDTH, CV_32FC3);

	QOpenGLShaderProgram *mapProgram;
	QOpenGLVertexArrayObject mapVAO;
	QOpenGLBuffer mapBuffer;
	float mapDepth = 2.4f;

	//QVector3D position = QVector3D(0.381632, 0.809913, -1.56082);
	//QVector3D direction = QVector3D(-0.116402, -0.159318, 0.980341);
	//QVector3D right = QVector3D(-0.993118, 0, -0.117117);
	//QVector3D up = QVector3D(-0.0186589, 0.987227, 0.158221);
	QVector3D position = QVector3D(0.0f, 0.0f, -2.0f);
	QVector3D direction = QVector3D(0.0f, 0.0f, 1.0f);
	QVector3D right = QVector3D(-1.0f, 0.0f, 0.0f);
	QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);
	//QVector3D position = QVector3D(4.80206966, 2.96364927, 3.91943645); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D direction = QVector3D(-0.699287236, -0.425939441, -0.574084401); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D right = QVector3D(0.633906007, 0.000000000, -0.773410082); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D up = QVector3D(-0.329425871, 0.904751360, -0.270005584); // QVector3D(0.0, 1.0, 0.0);
	float FoV = 45.0;

	int attrLocationVertex;
	int attrLocationColor;

	Map map;

	QOpenGLDebugLogger *logger;

	QPoint mouseStart = QPoint(0.0, 0.0);

	float horizontalAngle = 0.0f; // 3.14f;
	float verticalAngle = 0.0f; // look at the horizon

	float speed = 0.05f; // 3 units / second
	float mouseSpeed = 0.005f;
	float deltaTime = 1.0f;

	const float CAMERA_SPEED = 0.05f;

	// Kinect timer
	int kinectTimerId;

	// Private Helpers
	void updateInfo();
	void printInformation();
	void printVersionInformation();
};

#endif // MAIN_GL_WIDGET_H
