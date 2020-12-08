#include "MainGLWidget.h"
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QOpenGLShaderProgram>
#include <QtMath>
#include <QOpenGLExtraFunctions>
#include <QtGlobal>
#include <QOpenGLDebugLogger>
#include <ctime>
#include <sstream>
#include <QFileInfo>
#include "vertex.h"
#include "filters/ThresholdFilter.h"

#ifdef APPLE
#include "KinectManager_MacOS.h"
#else
#include "KinectManager_Windows.h"
#endif

#define KM KinectManager::instance()

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

/*
static const GLfloat sg_cube_vertices[] = {
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};*/

// e.g. Valid point: {X=-1.55726588 Y=1.33272767 Z=2.19400001 }
static const GLfloat map_plane_vertices[] = {
	-2.0f, -2.0f, 0.5f,
	2.0f, -2.0f, 0.5f,
	2.0f, 2.0f, 0.5f,
	-2.0f, -2.0f, 0.5f,
	-2.0f, 2.0f, 0.5f,
	2.0f, 2.0f, 0.5f,
};

#define MAKE_RECT(x, y, z, w, h, r, g, b) { \
	{ x - w/2, y - h/2, z, r, g, b }, \
	{ x + w/2, y - h/2, z, r, g, b }, \
	{ x + w/2, y + h/2, z, r, g, b }, \
	{ x - w/2, y - h/2, z, r, g, b }, \
	{ x - w/2, y + h/2, z, r, g, b }, \
	{ x + w/2, y + h/2, z, r, g, b }, \
}
float planeWidth = 1.2f;
float planeHeight = 0.6f;
float planeX = -0.25; 
static const GLfloat map_vertices[][6][6] = {
	//MAKE_RECT(0.0f, 0.0f, 0.50f, 4.0f, 4.0f, 0.5f, 0.5f, 0.5f), // Plane
	//MAKE_RECT(0.0f, 0.0f, 0.4999f, 1.0f, 1.0f, 0.0f, 0.7f, 0.0f), // Building 1
	//MAKE_RECT(1.0f, 1.0f, 0.4999f, 0.5f, 0.5f, 0.0f, 1.0f, 0.2f), // Building 2
	MAKE_RECT(-0.2f, 0.25f, 0.0f, 1.2f, 0.65f, 0.5f, 0.5f, 0.5f), // Plane
	MAKE_RECT(0.0f, 0.0f, +0.0001f, 1.0f, 1.0f, 0.0f, 0.7f, 0.0f), // Building 1
};
/*static const GLfloat map_vertices[][6][6] = { {
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right

0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, // Bottom-left
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f  // Top-left
} };*/
/*static const std::vector<std::vector<float[6]>>map_vertices{
	MAKE_RECT(0.0f, 0.0f, 4.0f, 4.0f, 0.5f, 0.5f, 0.5f), // Plane
	MAKE_RECT(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.7f, 0.0f), // Building 1
};*/

MainGLWidget::MainGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	// Create pipeline
	pipeline.addProcessingFilter(thresholdFilter);

	// Make window activ to recieve key strokes 
	// and be able to handle them in here
	setFocus();

	updateInfo();
}

MainGLWidget::~MainGLWidget()
{
}

void MainGLWidget::initializeGL()
{
	// Initialize OpenGL Backend
	initializeOpenGLFunctions();
	//connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
	printVersionInformation();

	// Set global information
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_DEBUG_OUTPUT);

	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setOption(QSurfaceFormat::DebugContext);

	//QOpenGLContext *ctx = QOpenGLContext::currentContext();
	auto ctx = (QOpenGLContext*)context();
	ctx->setFormat(format);
	logger = new QOpenGLDebugLogger(this);

	logger->initialize(); // initializes in the current context, i.e. ctx

	// Cull triangles which normal is not towards the camera = you cannnot go inside objects
	//glEnable(GL_CULL_FACE);

	{
		//QFileInfo info("./shaders/point_cloud.vert");
		//qDebug() << info.absoluteFilePath();
		kinectProgram = new QOpenGLShaderProgram();
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/point_cloud.vert");
		kinectProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/point_cloud.frag");
		kinectProgram->link();

		attrLocationVertex = kinectProgram->attributeLocation("vertex");
		attrLocationColor = kinectProgram->attributeLocation("color");

		kinectProgram->bind();
		kinectProgram->setAttributeValue("color", QVector3D(1.0, 0.0, 0.0));
		kinectProgram->release();
	}

	{
		mapProgram = new QOpenGLShaderProgram();
		mapProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/map_view.vert");
		mapProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/map_view.frag");
		mapProgram->link();

		mapVAO.create();
		mapVAO.bind();

		mapBuffer.create();
		mapBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		mapBuffer.bind();
		mapBuffer.allocate(map_vertices, sizeof(map_vertices));
		mapBuffer.release();

		mapVAO.release();
		mapProgram->release();
	}

	kinectTimerId = startTimer(KinectManager::DELAY_MS);

	updateInfo();
}

void MainGLWidget::resizeGL(int width, int height)
{
	// Currently we are not handling width/height changes.
	(void)width;
	(void)height;
}

void MainGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!kinectDepthBuffer.isCreated()) {
		return;
	}

	QMatrix4x4 view;
	view.lookAt(position, position + direction, QVector3D(0, 1, 0));

	QMatrix4x4 projection;
	projection.perspective(FoV, width() / (GLfloat)height(), 0.1, 100.0);

	mapProgram->bind();
	{
		QMatrix4x4 model;
		model.translate(0.0f, 0.0f, mapDepth);
		mapProgram->setUniformValue("model", model);

		mapProgram->setUniformValue("view", view);
		mapProgram->setUniformValue("projection", projection);

		auto attrLocationVertex = mapProgram->attributeLocation("vertex");
		auto attrLocationColor = mapProgram->attributeLocation("color");

		mapVAO.bind();

		mapBuffer.bind();
		glEnableVertexAttribArray(attrLocationVertex);
		glVertexAttribPointer(attrLocationVertex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(attrLocationColor);
		glVertexAttribPointer(attrLocationColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		for (int i = 0; i < sizeof(map_vertices) / 6 / 6; i++) {
			glDrawArrays(GL_TRIANGLES, i * 6, 6);
		}
		glDisableVertexAttribArray(attrLocationVertex);
		glDisableVertexAttribArray(attrLocationColor);

		mapBuffer.release();

		mapVAO.release();
	}
	mapProgram->release();

	kinectProgram->bind();
	{
		kinectProgram->setUniformValue("view", view);
		kinectProgram->setUniformValue("projection", projection);

		kinectVAO.bind();

		glEnableVertexAttribArray(attrLocationVertex);
		kinectDepthBuffer.bind();
		glVertexAttribPointer(attrLocationVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		kinectDepthBuffer.release();

		glEnableVertexAttribArray(attrLocationColor);
		kinectRGBBuffer.bind();
		glVertexAttribPointer(attrLocationColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, kinectDepthBuffer.size());

		glDisableVertexAttribArray(attrLocationVertex);
		glDisableVertexAttribArray(attrLocationColor);

		kinectDepthBuffer.release();
		kinectRGBBuffer.release();

		kinectVAO.release();
	}
	kinectProgram->release();
}

void MainGLWidget::timerEvent(QTimerEvent *)
{
	auto frameResult = KM.AcquireLatestFrame();
	if (((long)frameResult) >= 0) {
		kinectProgram->bind();

		if (kinectVAO.isCreated()) {
			kinectVAO.destroy();
		}
		kinectVAO.create();
		kinectVAO.bind();

		// Get depth + rgb data
		KM.writeDepthData(depthMat.data);
		KM.writeRgbData(rgbMat.data);

		// Run pipeline
		pipeline.process(&depthMat, &rgbMat);

		// Copy to GPU buffer
		kinectDepthBuffer.create();
		kinectDepthBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		kinectDepthBuffer.bind();
		kinectDepthBuffer.allocate(KM.getDepthSize());
		auto depthBufferDest = kinectDepthBuffer.mapRange(0, KM.getDepthSize(), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
		memcpy_s(depthBufferDest, KM.getDepthSize(), depthMat.data, KM.getDepthSize());
		//KM.writeDepthData(depthBufferDest);
		kinectDepthBuffer.unmap();
		kinectDepthBuffer.release();

		kinectRGBBuffer.create();
		kinectRGBBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
		kinectRGBBuffer.bind();
		kinectRGBBuffer.allocate(KM.getRgbSize());
		auto rgbBufferDest = kinectRGBBuffer.mapRange(0, KM.getRgbSize(), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
		memcpy_s(rgbBufferDest, KM.getRgbSize(), rgbMat.data, KM.getRgbSize());
		//KM.writeRgbData(rgbBufferDest);
		kinectRGBBuffer.unmap();
		kinectRGBBuffer.release();

		kinectVAO.release();
		kinectProgram->release();

		GLenum glErr;
		while ((glErr = glGetError()) != GL_NO_ERROR)
		{
			qDebug() << glErr;
			const QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
			for (const QOpenGLDebugMessage &message : messages)
				qDebug() << message;
		}

		update();
	}
	KM.ReleaseLatestFrame();
}

void MainGLWidget::keyPressEvent(QKeyEvent *event) {
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
	{
		std::time_t result = std::time(nullptr);
		std::stringstream path;
		path << "C:\\Users\\Kevin Bein\\Downloads\\" << result << ".png";
		KM.saveRGBImage(path.str());

		std::stringstream message;
		message << "Saved RGB snapshot at " << path.str() << "";
		qDebug() << message.str().c_str();

		std::stringstream title;
		title << "CDP Tests - " << message.str();
		window()->setWindowTitle(title.str().c_str());
	}
	break;

	case Qt::Key::Key_2:
		mapDepth -= 0.2f;
		break;

	case Qt::Key::Key_3:
		mapDepth += 0.2f;
		break;
	}
	//qDebug() << "press key: " << event->key() << " (" << event->text() << ") ";
	event->accept();
	updateInfo();
}

void MainGLWidget::mousePressEvent(QMouseEvent *event) {
	mouseStart = event->pos();
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent *) {
	setFocus();
}

void MainGLWidget::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() == Qt::LeftButton) {
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

		updateInfo();
	}
	event->accept();
}

void MainGLWidget::wheelEvent(QWheelEvent *event) {
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical) {
		FoV -= 5.0 * numSteps;

		updateInfo();
	}
	event->accept();
}

void MainGLWidget::teardownGL()
{
	killTimer(kinectTimerId);

	kinectDepthBuffer.destroy();
	kinectRGBBuffer.destroy();
	kinectVAO.destroy();

	delete kinectProgram;
}

void MainGLWidget::updateInfo()
{
	printInformation();
	QWidget::update();
}

void MainGLWidget::printInformation()
{
	std::stringstream output;

	output << "QVector3D position    = QVector3D(" << position.x() << ", " << position.y() << ", " << position.z() << ");" << std::endl;
	output << "QVector3D direction   = QVector3D(" << direction.x() << ", " << direction.y() << ", " << direction.z() << ");" << std::endl;
	output << "QVector3D right       = QVector3D(" << right.x() << ", " << right.y() << ", " << right.z() << ");" << std::endl;
	output << "QVector3D up          = QVector3D(" << up.x() << ", " << up.y() << ", " << up.z() << ");" << std::endl;
	output << "float FoV             = " << FoV << ";" << std::endl;
	output << "float horizontalAngle = " << horizontalAngle << ";" << std::endl;
	output << "float verticalAngle   = " << verticalAngle << ";" << std::endl;

	output << "" << std::endl;

	emit setOutput(QString::fromStdString(output.str()));
}

void MainGLWidget::printVersionInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;

	// Get Version Information
	auto ctx = (QOpenGLContext*) this->context();
	glType = (ctx->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
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

void MainGLWidget::recordVideo() {
	//if (!KM.startVideoRecording()) {
	//	return;
	//}

	emit startedRecordingVideo();
	//emit stoppedRecordingVideo();
}
