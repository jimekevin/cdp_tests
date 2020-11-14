#include "window.h"
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QOpenGLShaderProgram>
#include <QtMath>
#include "vertex.h"
#include "KinectManager.h"

// Create a colored triangle
static const Vertex sg_vertexes[] = {
  Vertex(QVector3D(0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
  Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
  Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

static const Vertex sg_cube[] = {
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.583f,  0.771f,  0.014f)),
	Vertex(QVector3D(-1.0f,-1.0f, 1.0f), QVector3D(0.609f,  0.115f,  0.436f)),
	Vertex(QVector3D(-1.0f, 1.0f, 1.0f), QVector3D(0.327f,  0.483f,  0.844f)),
	Vertex(QVector3D(1.0f, 1.0f,-1.0f),  QVector3D(0.822f,  0.569f,  0.201f)),
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.435f,  0.602f,  0.223f)),
	Vertex(QVector3D(-1.0f, 1.0f,-1.0f), QVector3D(0.310f,  0.747f,  0.185f)),
	Vertex(QVector3D(1.0f,-1.0f, 1.0f),  QVector3D(0.597f,  0.770f,  0.761f)),
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.559f,  0.436f,  0.730f)),
	Vertex(QVector3D(1.0f,-1.0f,-1.0f),  QVector3D(0.359f,  0.583f,  0.152f)),
	Vertex(QVector3D(1.0f, 1.0f,-1.0f),  QVector3D(0.483f,  0.596f,  0.789f)),
	Vertex(QVector3D(1.0f,-1.0f,-1.0f),  QVector3D(0.559f,  0.861f,  0.639f)),
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.195f,  0.548f,  0.859f)),
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.014f,  0.184f,  0.576f)),
	Vertex(QVector3D(-1.0f, 1.0f, 1.0f), QVector3D(0.771f,  0.328f,  0.970f)),
	Vertex(QVector3D(-1.0f, 1.0f,-1.0f), QVector3D(0.406f,  0.615f,  0.116f)),
	Vertex(QVector3D(1.0f,-1.0f, 1.0f),  QVector3D(0.676f,  0.977f,  0.133f)),
	Vertex(QVector3D(-1.0f,-1.0f, 1.0f), QVector3D(0.971f,  0.572f,  0.833f)),
	Vertex(QVector3D(-1.0f,-1.0f,-1.0f), QVector3D(0.140f,  0.616f,  0.489f)),
	Vertex(QVector3D(-1.0f, 1.0f, 1.0f), QVector3D(0.997f,  0.513f,  0.064f)),
	Vertex(QVector3D(-1.0f,-1.0f, 1.0f), QVector3D(0.945f,  0.719f,  0.592f)),
	Vertex(QVector3D(1.0f,-1.0f, 1.0f),  QVector3D(0.543f,  0.021f,  0.978f)),
	Vertex(QVector3D(1.0f, 1.0f, 1.0f),  QVector3D(0.279f,  0.317f,  0.505f)),
	Vertex(QVector3D(1.0f,-1.0f,-1.0f),  QVector3D(0.167f,  0.620f,  0.077f)),
	Vertex(QVector3D(1.0f, 1.0f,-1.0f),  QVector3D(0.347f,  0.857f,  0.137f)),
	Vertex(QVector3D(1.0f,-1.0f,-1.0f),  QVector3D(0.055f,  0.953f,  0.042f)),
	Vertex(QVector3D(1.0f, 1.0f, 1.0f),  QVector3D(0.714f,  0.505f,  0.345f)),
	Vertex(QVector3D(1.0f,-1.0f, 1.0f),  QVector3D(0.783f,  0.290f,  0.734f)),
	Vertex(QVector3D(1.0f, 1.0f, 1.0f),  QVector3D(0.722f,  0.645f,  0.174f)),
	Vertex(QVector3D(1.0f, 1.0f,-1.0f),  QVector3D(0.302f,  0.455f,  0.848f)),
	Vertex(QVector3D(-1.0f, 1.0f,-1.0f), QVector3D(0.225f,  0.587f,  0.040f)),
	Vertex(QVector3D(1.0f, 1.0f, 1.0f),  QVector3D(0.517f,  0.713f,  0.338f)),
	Vertex(QVector3D(-1.0f, 1.0f,-1.0f), QVector3D(0.053f,  0.959f,  0.120f)),
	Vertex(QVector3D(-1.0f, 1.0f, 1.0f), QVector3D(0.393f,  0.621f,  0.362f)),
	Vertex(QVector3D(1.0f, 1.0f, 1.0f),  QVector3D(0.673f,  0.211f,  0.457f)),
	Vertex(QVector3D(-1.0f, 1.0f, 1.0f), QVector3D(0.820f,  0.883f,  0.371f)),
	Vertex(QVector3D(1.0f,-1.0f, 1.0f),  QVector3D(0.982f,  0.099f,  0.879f))
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
	glClearDepth(1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Cull triangles which normal is not towards the camera = you cannnot go inside objects
	//glEnable(GL_CULL_FACE);

	//perspectiveGL(45, KM.WIDTH / (GLdouble)KM.HEIGHT, 0.1, 1000);

	//QVector4D uv(xy.x() / m_w * 2 - 1, -(xy.y() / m_h * 2 - 1), 1, 1);
	//QVector3D sv = m_proj.inverted() * uv;

	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)
		kinectProgram = new QOpenGLShaderProgram();
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/point_cloud.vert");
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/point_cloud.frag");
		kinectProgram->link();

		kinectProgram->bind();

		// Set transformation
		//view.lookAt(
		//	//QVector3D(0, 0, 2), // Camera is at (4,3,3), in World Space
		//	QVector3D(2, 2, -1),
		//	QVector3D(0, 0, 0), // and looks at the origin
		//	QVector3D(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		//);
		//projection.perspective(45.0, KM.WIDTH / (GLdouble) KM.HEIGHT, 0.1, 100.0);

		kinectProgram->setAttributeValue("color", QVector3D(1.0, 0.0, 0.0));

		// Create Buffer (Do not release until VAO is created)
		m_vertex.create();
		m_vertex.bind();
		m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		m_vertex.allocate(sg_cube, sizeof(sg_cube));

		// Create Vertex Array Object
		m_object.create();
		m_object.bind();
		kinectProgram->enableAttributeArray(0);
		kinectProgram->enableAttributeArray(1);
		kinectProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		kinectProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		m_object.release();
		m_vertex.release();
		kinectProgram->release();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	kinectProgram->bind();
	{
		QMatrix4x4 view;
		view.lookAt(position, position + direction, QVector3D(0, 1, 0));
		kinectProgram->setUniformValue("view", view);

		QMatrix4x4 projection;
		projection.perspective(FoV, KM.WIDTH / (GLdouble)KM.HEIGHT, 0.1, 100.0);
		kinectProgram->setUniformValue("projection", projection);

		m_object.bind();
		glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_cube) / sizeof(sg_cube[0]));
		m_object.release();

		kinectDepthBuffer.bind();
		glDrawArrays(GL_POINTS, 0, kinectDepthBuffer.size());
		kinectDepthBuffer.release();
	}
	kinectProgram->release();
}

void Window::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key::Key_Up:
	case Qt::Key::Key_W:
		//view.translate(0, CAMERA_SPEED, 0);
		position += up * deltaTime * speed;
		break;
	case Qt::Key::Key_Down:
	case Qt::Key::Key_S:
		//view.translate(0, -CAMERA_SPEED, 0);
		position -= up * deltaTime * speed;
		break;
	case Qt::Key::Key_Right:
	case Qt::Key::Key_D:
		//view.translate(CAMERA_SPEED, 0, 0);
		position += right * deltaTime * speed;
		break;
	case Qt::Key::Key_Left:
	case Qt::Key::Key_A:
		//view.translate(-CAMERA_SPEED, 0, 0);
		position -= right * deltaTime * speed;
		break;
	case Qt::Key::Key_Q:
		//view.translate(CAMERA_SPEED, 0, 0);
		position += direction * deltaTime * speed;
		break;
	case Qt::Key::Key_E:
		//view.translate(-CAMERA_SPEED, 0, 0);
		position -= direction * deltaTime * speed;
		break;
	} 
	//qDebug() << "press key: " << event->key() << " (" << event->text() << ") ";
	event->accept();
}

void Window::mousePressEvent(QMouseEvent *event) {
	mouseStart = event->pos();
}

void Window::mouseReleaseEvent(QMouseEvent *event) {
}

void Window::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() == Qt::LeftButton) {
		// Compute new orientation
		const auto deltaTime = 1.0;
		auto mouseX = event->pos().x() - mouseStart.x();
		auto mouseY = event->pos().y() - mouseStart.y();
		qDebug() << "Mouse move: " << mouseX << ", " << mouseY;
		horizontalAngle += mouseSpeed * deltaTime * mouseX;
		verticalAngle += mouseSpeed * deltaTime * mouseY;
		direction = QVector3D(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		right = QVector3D(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		up = QVector3D::crossProduct(right, direction);

		mouseStart = event->pos();
		
		update();
	}
	event->accept();
}

void Window::wheelEvent(QWheelEvent *event) {
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical) {
		FoV -= 5.0 * numSteps;

		update();
	}
	event->accept();
}

void Window::timerEvent(QTimerEvent *event)
{
	//qDebug() << "Update...";	
	// New kinect frames arrived

	IMultiSourceFrame* frame = NULL;
	if (SUCCEEDED(KM.reader->AcquireLatestFrame(&frame))) {
		//qDebug() << "New frames!";

		kinectProgram->bind();

		// Create Vertex Array Object
		if (kinectVAO.isCreated()) {
			kinectVAO.destroy();
		}
		kinectVAO.create();
		kinectVAO.bind();

		kinectDepthBuffer.create();
		kinectDepthBuffer.bind();
		kinectDepthBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		KM.getDepthData(frame, &kinectDepthBuffer);
		auto depthBufferId = kinectDepthBuffer.bufferId();

		kinectRGBBuffer.create();
		kinectRGBBuffer.bind();
		kinectRGBBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		KM.getRgbData(frame, &kinectRGBBuffer);
		auto RGBBufferId = kinectRGBBuffer.bufferId();

		kinectProgram->enableAttributeArray(0);
		kinectProgram->enableAttributeArray(1);
		kinectProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
		kinectProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		kinectVAO.release();
		kinectDepthBuffer.release();
		//kinectRGBBuffer.release();
		kinectProgram->release();

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