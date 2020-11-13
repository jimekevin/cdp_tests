#pragma once

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

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
	void timerEvent(QTimerEvent *event);
protected slots:
	void teardownGL();

private:
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;

	QOpenGLBuffer depthBuffer;
	QOpenGLBuffer rgbBuffer;
	QOpenGLVertexArrayObject kinectVAO;
	QOpenGLShaderProgram *kinectProgram;

	// Kinect timer
	int kinectTimerId;

	// Private Helpers
	void printVersionInformation();
};

