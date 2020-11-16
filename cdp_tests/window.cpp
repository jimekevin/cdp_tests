#include "window.h"
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QOpenGLShaderProgram>
#include <QtMath>
#include <QOpenGLExtraFunctions>
#include <QtGlobal>
#include <QOpenGLDebugLogger>
#include <ctime>
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

static const GLfloat sg_cube_vertices[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};
static const GLfloat sg_cube_color[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
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
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	glEnable(GL_DEBUG_OUTPUT);

	QSurfaceFormat format;
	// asks for a OpenGL 3.2 debug context using the Core profile
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setOption(QSurfaceFormat::DebugContext);

	QOpenGLContext *ctx = QOpenGLContext::currentContext();
	ctx->setFormat(format);
	logger = new QOpenGLDebugLogger(this);

	logger->initialize(); // initializes in the current context, i.e. ctx

	// Cull triangles which normal is not towards the camera = you cannnot go inside objects
	//glEnable(GL_CULL_FACE);

	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)
		kinectProgram = new QOpenGLShaderProgram();
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/point_cloud.vert");
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/point_cloud.frag");
		kinectProgram->link();

		attrLocationVertex = kinectProgram->attributeLocation("vertex");
		attrLocationColor = kinectProgram->attributeLocation("color");

		kinectProgram->bind();

		// Default color
		// TODO: Remove
		kinectProgram->setAttributeValue("color", QVector3D(1.0, 0.0, 0.0));

		// Create Buffer (Do not release until VAO is created)
		//m_vertex.create();
		//m_vertex.bind();
		//m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//m_vertex.allocate(sg_cube, sizeof(sg_cube));

		// Create Vertex Array Object
		//m_object.create();
		//m_object.bind();
		//kinectProgram->enableAttributeArray(attrLocationVertex);
		//kinectProgram->enableAttributeArray(attrLocationColor);
		//kinectProgram->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		//kinectProgram->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
		//kinectProgram->setAttributeBuffer(attrLocationVertex, GL_FLOAT, 0, 3, 0);
		//kinectProgram->setAttributeBuffer(attrLocationColor, GL_FLOAT, 0, 3, 0);

		// Release (unbind) all
		//m_object.release();
		//m_vertex.release();
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
	GLenum glErr;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	if (!kinectDepthBuffer.isCreated()) {
		return;
	}
	//if (!kinectBuffer.isCreated()) {
	//	return;
	//}

	kinectProgram->bind();
	{
		QMatrix4x4 view;
		view.lookAt(position, position + direction, QVector3D(0, 1, 0));
		kinectProgram->setUniformValue("view", view);

		QMatrix4x4 projection;
		projection.perspective(FoV, KM.WIDTH / (GLdouble)KM.HEIGHT, 0.1, 100.0);
		kinectProgram->setUniformValue("projection", projection);

		//m_object.bind();
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_cube) / sizeof(sg_cube[0]));
		//m_object.release();

		kinectVAO.bind();

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);

		//kinectDepthBuffer.bind();
		//glDrawArrays(GL_POINTS, 0, KM.WIDTH * KM.HEIGHT);
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_cube_vertices) / sizeof(sg_cube_vertices[0]));
		//glVertexPointer(3, GL_FLOAT, 0, NULL);

		//kinectRGBBuffer.bind();
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_cube_color) / sizeof(sg_cube_color[0]));
		//glColorPointer(3, GL_FLOAT, 0, NULL);
		//glDrawArrays(GL_POINTS, 0, KM.WIDTH * KM.HEIGHT);

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_cube_vertices) / sizeof(sg_cube_vertices[0]));

		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);

		glEnableVertexAttribArray(attrLocationVertex);
		kinectDepthBuffer.bind();
		glVertexAttribPointer(attrLocationVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		kinectDepthBuffer.release();
		glEnableVertexAttribArray(attrLocationColor);
		kinectRGBBuffer.bind();
		glVertexAttribPointer(attrLocationColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//auto glErr = glGetError();
		auto depthBufferSize = kinectDepthBuffer.size();
		glDrawArrays(GL_POINTS, 0, depthBufferSize);
		//glErr = glGetError();
		
		//kinectRGBBuffer.bind();
		//glEnableVertexAttribArray(attrLocationColor);
		//glVertexAttribPointer(attrLocationColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//kinectBuffer.bind();
		//glEnableVertexAttribArray(attrLocationVertex);
		//glEnableVertexAttribArray(attrLocationColor);
		//glVertexAttribPointer(attrLocationVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glVertexAttribPointer(attrLocationColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//auto size = kinectBuffer.size();
		//glDrawArrays(GL_POINTS, 0, kinectBuffer.size());

		glDisableVertexAttribArray(attrLocationVertex);
		glDisableVertexAttribArray(attrLocationColor);

		//kinectBuffer.release();
		kinectDepthBuffer.release();
		kinectRGBBuffer.release();

		kinectVAO.release();
	}
	kinectProgram->release();

	//qDebug() << "QVector3D position  = QVector3D(" << position.x() << ", " << position.y() << ", " << position.z() << ");";
	//qDebug() << "QVector3D direction = QVector3D(" << direction.x() << ", " << direction.y() << ", " << direction.z() << ");";
	//qDebug() << "QVector3D right     = QVector3D(" << right.x() << ", " << right.y() << ", " << right.z() << ");";
	//qDebug() << "QVector3D up        = QVector3D(" << up.x() << ", " << up.y() << ", " << up.z() << ");";
	//qDebug() << "float horizontalAngle = " << horizontalAngle << ";";
	//qDebug() << "float verticalAngle = " << verticalAngle << ";";
	//qDebug() << "";
}

void Window::timerEvent(QTimerEvent *event)
{
	GLenum glErr;
	//qDebug() << "Update...";	
	// New kinect frames arrived

	IMultiSourceFrame* frame = NULL;
	auto frameResult = KM.reader->AcquireLatestFrame(&frame);
	if (SUCCEEDED(frameResult)) {
		//qDebug() << "New frames!";

		kinectProgram->bind();

		// Create Vertex Array Object
		if (kinectVAO.isCreated()) {
			kinectVAO.destroy();
		}
		kinectVAO.create();
		kinectVAO.bind();

		//kinectBuffer.create();
		//kinectBuffer.bind();
		//kinectBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		//KM.getDepthAndRGBData(frame, &kinectBuffer);

		//if (kinectDepthBuffer.isCreated()) kinectDepthBuffer.destroy();
		kinectDepthBuffer.create();
		kinectDepthBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		kinectDepthBuffer.bind();
		auto size = kinectDepthBuffer.size();
		KM.getDepthData(frame, &kinectDepthBuffer);
		size = kinectDepthBuffer.size();
		//kinectDepthBuffer.allocate(sg_cube_vertices, sizeof(sg_cube_vertices) / sizeof(sg_cube_vertices[0])); 
		kinectDepthBuffer.release();

		auto result = kinectRGBBuffer.create();
		kinectRGBBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		result = kinectRGBBuffer.bind();
		auto size2 = kinectRGBBuffer.size();
		KM.getRgbData(frame, &kinectRGBBuffer);
		size2 = kinectRGBBuffer.size();
		kinectRGBBuffer.release();
		//kinectRGBBuffer.allocate(sg_cube_color, sizeof(sg_cube_color) / sizeof(sg_cube_color[0]));

		//kinectDepthBuffer.bind();
		//kinectProgram->enableAttributeArray(0);
		//kinectProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
		//kinectDepthBuffer.release();

		//kinectRGBBuffer.bind();
		//kinectProgram->enableAttributeArray(1);
		//kinectProgram->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);
		//kinectRGBBuffer.release();
		//kinectProgram->enableAttributeArray(1);
		//kinectProgram->setAttributeBuffer(1, GL_FLOAT, 3, 3, 0); // Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		kinectVAO.release();
		//kinectBuffer.release();
		kinectProgram->release();

		while ((glErr = glGetError()) != GL_NO_ERROR)
		{
			qDebug() << glErr;

			const QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
			for (const QOpenGLDebugMessage &message : messages)
				qDebug() << message;
			//GLint maxMsgLen = 0;
			//glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
			//std::vector<GLchar> msgData(1 * maxMsgLen);
			//std::vector<GLenum> sources(1);
			//std::vector<GLenum> types(1);
			//std::vector<GLenum> severities(1);
			//std::vector<GLuint> ids(1);
			//std::vector<GLsizei> lengths(1);

			//GLuint numFound = glGetDebugMessageLog(1, maxMsgLen, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

			//sources.resize(numFound);
			//types.resize(numFound);
			//severities.resize(numFound);
			//ids.resize(numFound);
			//lengths.resize(numFound);

			//std::vector<std::string> messages;
			//messages.reserve(numFound);

			//std::vector<GLchar>::iterator currPos = msgData.begin();
			//for (size_t msg = 0; msg < lengths.size(); ++msg)
			//{
			//	messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
			//	currPos = currPos + lengths[msg];
			//}
		}

		// Force painting
		update();
	}
	if (frame) {
		frame->Release();
	}
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

	case Qt::Key::Key_1:
		std::time_t result = std::time(nullptr);
		std::stringstream path;
		path << "C:\\Users\\Kevin Bein\\Downloads\\" << result << ".png";
		KM.saveRGBImage(path.str());
		qDebug() << "Write RGB to disk: " << path.str().c_str();
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
		//qDebug() << "Mouse move: " << mouseX << ", " << mouseY;
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

	qDebug() << "Qt Version: " << QT_VERSION_STR;

	// qPrintable() will print our QString w/o quotes around it.
	qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}