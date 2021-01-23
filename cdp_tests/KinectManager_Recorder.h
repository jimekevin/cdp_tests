#ifndef KINECTMANAGER_RECORDER_H
#define KINECTMANAGER_RECORDER_H

#include <QtGui/QOpenGLBuffer>

class KinectManager {

public:
  const static int DELAY_MS = 33;
  const static int WIDTH = 512;
  const static int HEIGHT = 424;
  const static int COLORWIDTH = 1920;
  const static int COLORHEIGHT = 1080;

  unsigned char rgbimage[COLORWIDTH * COLORHEIGHT * 4]{};

private:
  std::string serial;
  bool startedStreams = false;

  KinectManager() = default; // Disallow instantiation outside of the class.

public:

  KinectManager(const KinectManager&) = delete;
  KinectManager& operator=(const KinectManager &) = delete;
  KinectManager(KinectManager &&) = delete;
  KinectManager& operator=(KinectManager &&) = delete;

  static auto& instance() {
      static KinectManager i;
      return i;
  }

  int initialize();
  int initialize(std::string videoSource);
  void terminate();

  long AcquireLatestFrame();
  void ReleaseLatestFrame();

  const int getDepthSize();
  const int getRgbSize();

  void writeDepthData(void *dest);
  void writeRgbData(void *dest);

  void saveRGBImage(const std::string& path);
  void saveRGBImage(const std::string& path, unsigned char *input, int width, int height);
  void startVideoRecording(const std::string& path);
  void stopVideoRecording();
};

#endif // KINECTMANAGER_RECORDER_H
