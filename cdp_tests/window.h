#pragma once

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLDebugLogger>

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow,
	protected QOpenGLFunctions
{
	Q_OBJECT

		// OpenGL Events
public:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

protected:
	void keyPressEvent(QKeyEvent *event);
	void timerEvent(QTimerEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

protected slots:
	void teardownGL();

private:
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;

	QOpenGLShaderProgram *kinectProgram;
	QOpenGLVertexArrayObject kinectVAO;
	QOpenGLBuffer kinectBuffer;
	QOpenGLBuffer kinectDepthBuffer;
	QOpenGLBuffer kinectRGBBuffer;
	unsigned int* kinectDepthData;
	unsigned int* kinectRGBData;

	QVector3D position = QVector3D(0.381632, 0.809913, -1.56082);
	QVector3D direction = QVector3D(-0.116402, -0.159318, 0.980341);
	QVector3D right = QVector3D(-0.993118, 0, -0.117117);
	QVector3D up = QVector3D(-0.0186589, 0.987227, 0.158221);
	//QVector3D position = QVector3D(4.80206966, 2.96364927, 3.91943645); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D direction = QVector3D(-0.699287236, -0.425939441, -0.574084401); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D right = QVector3D(0.633906007, 0.000000000, -0.773410082); // QVector3D(0.0, 0.0, 0.0);
	//QVector3D up = QVector3D(-0.329425871, 0.904751360, -0.270005584); // QVector3D(0.0, 1.0, 0.0);
	float FoV = 45.0;

	int attrLocationVertex;
	int attrLocationColor;

	QOpenGLDebugLogger *logger;
		
	QPoint mouseStart = QPoint(0.0, 0.0);

	float horizontalAngle = 6.165; // 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = -0.16; // 0.0f;

	float speed = 0.05f; // 3 units / second
	float mouseSpeed = 0.005f;
	float deltaTime = 1.0f;

	const float CAMERA_SPEED = 0.05;

	// Kinect timer
	int kinectTimerId;

	// Private Helpers
	void printVersionInformation();
};

