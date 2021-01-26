#include "InputRecorder.h"

#include <ctime>

InputRecorder::ColorFrame::ColorFrame(char *rgbimage, RecorderInfo &recInfo)
	: rgbimage(rgbimage), recInfo(recInfo)
{
}

ULONG STDMETHODCALLTYPE InputRecorder::ColorFrame::Release() {
	//delete rgbimage;
	return 0;
}

HRESULT STDMETHODCALLTYPE InputRecorder::ColorFrame::CopyConvertedFrameDataToArray(UINT capacity, /* [annotation][size_is][out] */ _Out_writes_all_(capacity) BYTE *frameData, ColorImageFormat colorFormat) {
	memcpy(frameData, rgbimage, capacity);
	return S_OK;
}


InputRecorder::ColorFrameReference::ColorFrameReference(char *rgbimage, RecorderInfo &recInfo)
	: rgbimage(rgbimage), recInfo(recInfo)
{
}

HRESULT STDMETHODCALLTYPE InputRecorder::ColorFrameReference::AcquireFrame(IColorFrame **colorFrame) {
	auto cf = new ColorFrame(rgbimage, recInfo);
	*colorFrame = dynamic_cast<IColorFrame*>(cf);
	return S_OK;
}


InputRecorder::DepthFrame::DepthFrame(char *depth2rgb, char *depth2xyz, RecorderInfo &recInfo)
	: depth2rgb(depth2rgb), depth2xyz(depth2xyz), recInfo(recInfo)
{
}

HRESULT InputRecorder::DepthFrame::copyDepthData(UINT capacityRgb, BYTE *rgb, UINT capacityXyz, BYTE *xyz) {
	memcpy(rgb, depth2rgb, capacityRgb);
	memcpy(xyz, depth2xyz, capacityXyz);
	return S_OK;
}

ULONG STDMETHODCALLTYPE InputRecorder::DepthFrame::Release() {
	//delete depth2rgb;
	return 0;
}

HRESULT STDMETHODCALLTYPE InputRecorder::DepthFrame::AccessUnderlyingBuffer(UINT *capacity, UINT16 **buffer) {
	return S_FALSE; // 
	*capacity = recInfo.depthSize(); // AccessUnderlyingBuffer
	*buffer = new UINT16[*capacity];
	memcpy(*buffer, (UINT16*)depth2rgb, *capacity);
	//*buffer = (UINT16*) depth2rgb;
	return S_OK;
}


InputRecorder::DepthFrameReference::DepthFrameReference(char *depth2rgb, char *depth2xyz, RecorderInfo &recInfo)
	: depth2rgb(depth2rgb), depth2xyz(depth2xyz), recInfo(recInfo)
{
}

HRESULT STDMETHODCALLTYPE InputRecorder::DepthFrameReference::AcquireFrame(IDepthFrame **colorFrame) {
	auto cf = new DepthFrame(depth2rgb, depth2xyz, recInfo);
	*colorFrame = dynamic_cast<IDepthFrame*>(cf);
	return S_OK;
}


InputRecorder::MultiSourceFrame::MultiSourceFrame(char *depth2rgb, char *depth2xyz, char *rgbimage, RecorderInfo &recInfo)
	: depth2rgb(depth2rgb), depth2xyz(depth2xyz), rgbimage(rgbimage), recInfo(recInfo)
{
}

HRESULT STDMETHODCALLTYPE InputRecorder::MultiSourceFrame::get_ColorFrameReference(_COM_Outptr_  IColorFrameReference **colorFrameReference) {
	auto cfr = new ColorFrameReference(rgbimage, recInfo);
	*colorFrameReference = dynamic_cast<IColorFrameReference*>(cfr);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE InputRecorder::MultiSourceFrame::get_DepthFrameReference(_COM_Outptr_  IDepthFrameReference **depthFrameReference) {
	auto dfr = new DepthFrameReference(depth2rgb, depth2xyz, recInfo);
	*depthFrameReference = dynamic_cast<IDepthFrameReference*>(dfr);
	return S_OK;
}


InputRecorder::InputRecorder(std::string path) 
	: path(path) 
{
}

InputRecorder::~InputRecorder() {
	in.pop(); // gzip_decompressor
	in.pop(); // ifs
	ifs.close();
}

HRESULT STDMETHODCALLTYPE InputRecorder::AcquireLatestFrame(IMultiSourceFrame **multiSourceFrame) {
	if (in.peek() == EOF) {
		currentPosition = 0;
		// seek is not supported!! what now? reopen stream?
		//in.seekg(headerOffset);
		// not working also
		//ifs.seekg(0, std::ios::beg);

		// reopen file :>
		in.pop();
		boost::iostreams::close(in);
		ifs.close();
	}
	if (!ifs.is_open()) {
		ifs.open(path, std::ifstream::in | std::ifstream::binary);
		//in.push(boost::iostreams::gzip_decompressor());
		in.push(ifs);
		ifs.seekg(0, std::ios::beg);
	}

	//auto fsize = ifs.tellg();
	//ifs.seekg(0, std::ios::end);
	//fsize = ifs.tellg() - fsize;
	//ifs.seekg(0, std::ios::beg);

	if (depth2rgb != nullptr) {
		delete depth2rgb;
	}
	auto depth2rgbByteSize = recInfo.depth2rgbByteSize();
	depth2rgb = new char[depth2rgbByteSize];
	in.read(depth2rgb, depth2rgbByteSize);

	if (depth2xyz != nullptr) {
		delete depth2xyz;
	}
	auto depth2xyzByteSize = recInfo.depth2xyzByteSize();
	depth2xyz = new char[depth2xyzByteSize];
	in.read(depth2xyz, depth2xyzByteSize);

	if (rgbimage != nullptr) {
		delete rgbimage;
	}
	auto rgbimageByteSize = recInfo.rgbimageByteSize();
	rgbimage = new char[rgbimageByteSize];
	in.read(rgbimage, rgbimageByteSize);

	currentPosition++;

	auto msf = new MultiSourceFrame(depth2rgb, depth2xyz, rgbimage, recInfo);
	*multiSourceFrame = dynamic_cast<IMultiSourceFrame*>(msf);
	return S_OK;
}