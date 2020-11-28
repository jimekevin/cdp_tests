#ifndef KINECTMANAGER_MACOS_H
#define KINECTMANAGER_MACOS_H

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>

#include <QtGui/QOpenGLBuffer>

class KinectManager {

private:
  int frameCount = 0;
  long long lastFrameFetch = 0;

  libfreenect2::Freenect2 freenect2;
  libfreenect2::Freenect2Device *dev = nullptr;
  libfreenect2::PacketPipeline *pipeline = nullptr;
  libfreenect2::SyncMultiFrameListener *listener = nullptr;
  libfreenect2::FrameMap frames;

  libfreenect2::Frame *rgbFrame = nullptr;
  libfreenect2::Frame *irFrame = nullptr;
  libfreenect2::Frame *depthFrame = nullptr;

  std::string serial;
  bool startedStreams = false;

  KinectManager() = default;

public:
  const static int DELAY_MS = 33;
  const static int WIDTH = 512;
  const static int HEIGHT = 424;
  const static int COLORWIDTH = 1920;
  const static int COLORHEIGHT = 1080;

  KinectManager(const KinectManager&) = delete;
  KinectManager& operator=(const KinectManager &) = delete;
  KinectManager(KinectManager &&) = delete;
  KinectManager & operator=(KinectManager &&) = delete;

  static auto& instance() {
      static KinectManager i;
      return i;
  }

  int init();

  long AcquireLatestFrame();
  void ReleaseLatestFrame();

  void getDepthData(QOpenGLBuffer *glBuffer);
  void getRgbData(QOpenGLBuffer *glBuffer);
  //void getDepthAndRGBData(QOpenGLBuffer *glBuffer);

  void saveRGBImage(std::string path);
};

#endif // KINECTMANAGER_MACOS_H
