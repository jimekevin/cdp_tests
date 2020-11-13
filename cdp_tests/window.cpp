#include "window.h"
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QOpenGLShaderProgram>
#include "vertex.h"
#include "KinectManager.h"

// Create a colored triangle
static const Vertex sg_vertexes[] = {
  Vertex(QVector3D(0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
  Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
  Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};


/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void Window::initializeGL()
{
	// Initialize OpenGL Backend
	initializeOpenGLFunctions();
	connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
	printVersionInformation();

	// Set global information
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)
		m_program = new QOpenGLShaderProgram();
		//m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
		//m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
		m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/point_cloud.vert");
		m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/point_cloud.frag");
		m_program->link();

		m_program->bind();

		// Create Buffer (Do not release until VAO is created)
		m_vertex.create();
		m_vertex.bind();
		m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

		// Create Vertex Array Object
		m_object.create();
		m_object.bind();
		m_program->enableAttributeArray(0);
		m_program->enableAttributeArray(1);
		m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		m_object.release();
		m_vertex.release();
		m_program->release();
	}

	kinectTimerId = startTimer(KinectManager::DELAY_MS);
}

void Window::resizeGL(int width, int height)
{
	// Currently we are not handling width/height changes.
	(void)width;
	(void)height;
}

void Window::paintGL()
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT);

	// Render using our shader
	m_program->bind();
	{
		m_object.bind();
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
		glDrawArrays(GL_POINTS, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
		m_object.release();
	}
	m_program->release();
}

void Window::timerEvent(QTimerEvent *event)
{
	qDebug() << "Update...";	
	// New kinect frames arrived

	IMultiSourceFrame* frame = NULL;
	// TODO: Check for timer 30fps
	if (SUCCEEDED(KM.reader->AcquireLatestFrame(&frame))) {
		m_program->bind();

		QOpenGLBuffer depthBuffer;
		depthBuffer.create();
		depthBuffer.bind();
		depthBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		unsigned int* depthPtr;
		KM.getDepthData(frame, depthPtr);
		depthBuffer.allocate(depthPtr, sizeof(depthPtr));

		QOpenGLBuffer rgbBuffer;
		depthBuffer.create();
		depthBuffer.bind();
		depthBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		unsigned int* rgbPtr;
		KM.getRgbData(frame, rgbPtr);
		depthBuffer.allocate(rgbPtr, sizeof(rgbPtr));

		// Release (unbind) all
		depthBuffer.release();
		rgbBuffer.release();
		m_program->release();

		// Force painting
		update();
	}
	if (frame) {
		frame->Release();
	}
}

void Window::teardownGL()
{
	// Actually destroy our OpenGL information
	m_object.destroy();
	m_vertex.destroy();
	delete m_program;

	killTimer(kinectTimerId);
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;

	// Get Version Information
	glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	// Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch (format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	// qPrintable() will print our QString w/o quotes around it.
	qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}