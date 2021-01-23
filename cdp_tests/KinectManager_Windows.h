#ifndef APPLE

#ifndef KINECT_MANAGER_WINDOWS_H
#define KINECT_MANAGER_WINDOWS_H

#include <Kinect.h>
#include <opencv2/imgproc.hpp>
#include <QtGui/QOpenGLBuffer>

class KinectManager {
public:
  const static int DELAY_MS = 33;
  const static int WIDTH = 512;
  const static int HEIGHT = 424;
  const static int COLORWIDTH = 1920;
  const static int COLORHEIGHT = 1080;

  IMultiSourceFrameReader* reader;

  ColorSpacePoint depth2rgb[WIDTH * HEIGHT];  // Maps depth pixels to rgb pixels
  CameraSpacePoint depth2xyz[WIDTH * HEIGHT];
  unsigned char rgbimage[COLORWIDTH * COLORHEIGHT * 4]; // RGBA 0-255

private:
  KinectManager() {} // Disallow instantiation outside of the class.

  IKinectSensor *sensor;
  ICoordinateMapper *mapper; // Converts between depth, color, and 3d coordinates

  IColorFrameReader *colorFrameReader;
  IDepthFrameReader *depthFrameReader;

  int frameCount = 0;
  long long lastFrameFetch = 0;

  IMultiSourceFrame *lastFrame = nullptr;

  bool videoRecording = false;

public:
  KinectManager(const KinectManager&) = delete;
  KinectManager& operator=(const KinectManager &) = delete;
  KinectManager(KinectManager &&) = delete;
  KinectManager & operator=(KinectManager &&) = delete;

  static auto& instance() {
      static KinectManager test;
      return test;
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

#endif // KINECT_MANAGER_WINDOWS_H

#endif