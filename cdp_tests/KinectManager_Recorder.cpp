#include "KinectManager_Recorder.h"

int KinectManager::initialize()
{
    return 0;
}

int KinectManager::initialize(std::string videoSource)
{
    return 0;
}

void KinectManager::terminate()
{
}

long KinectManager::AcquireLatestFrame()
{
    return 0;
}

void KinectManager::ReleaseLatestFrame()
{
}

const int KinectManager::getDepthSize()
{
    return 0;
}

const int KinectManager::getRgbSize()
{
    return 0;
}


void KinectManager::writeDepthData(void *dest)
{
}

void KinectManager::writeRgbData(void *dest)
{
}

void KinectManager::saveRGBImage(const std::string& path)
{
}

void KinectManager::saveRGBImage(const std::string& path, unsigned char *input, int width, int height)
{
}

void KinectManager::startVideoRecording(const std::string& path)
{
}

void KinectManager::stopVideoRecording()
{
}

